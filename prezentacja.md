# Prezentacja Projektu - Przegląd Kodu (Code Walkthrough)

Ten dokument to "ściągawka" przygotowana z myślą o ułatwieniu obrony projektu. Znajdziesz tutaj przewodnik krok po kroku po architekturze rozwiązania oraz spis wszystkich istotnych plików wraz z wytłumaczeniem, jaką rolę pełnią w naszym ekosystemie oraz **kluczowe przykłady użytego w nich kodu**.

---

## 1. Serce Projektu (Silnik i Punkt Wejścia)

### `LivingWorld.cpp`

- **Rola:** Punkt startowy aplikacji (zawiera funkcję `main`).
- **Co tu się dzieje?**
  To tutaj powoływany jest do życia obiekt świata (`World world`). Mapa jest zasiedlana pierwszymi organizmami (np. Owcami, Wilkami) przy pomocy nowoczesnego `std::make_unique`. Na samym końcu pliku uruchamiany jest silnik graficzny (`OLCView`).

**Przykład z kodu:**

```cpp
// [MODERN C++] Bezpieczne zarządzanie pamięcią bez słowa "new"
world.addOrganism(std::make_unique<Sheep>(Position(1, 1), &world));
world.addOrganism(std::make_unique<Wolf>(Position(5, 5), &world));

// Uruchamianie silnika graficznego
int cellSize{ 20 };
OLCView view(world, cellSize);
if (view.Construct(worldSizeX * cellSize, worldSizeY * cellSize, 2, 2)) {
    view.Start();
}
```

### `World.h` / `World.cpp`

- **Rola:** Główny zarządca mapy i cyklu życia organizmów.
- **Co tu się dzieje?**
  Klasa ta zarządza rozmiarem mapy i trzyma wszystkie organizmy w kontenerze polimorficznym `std::vector<std::unique_ptr<Organism>>`. Najważniejsza metoda to **`makeTurn()`**, zrealizowana przy pomocy wielu idiomów Modern C++.

**Przykład z kodu (Algorytmy STL w `makeTurn`):**

```cpp
// Sortowanie przy użyciu Wyrażenia Lambda (najszybsi mają tury pierwsi)
std::sort(organisms.begin(), organisms.end(), [](const unique_ptr<Organism>& a, const unique_ptr<Organism>& b) {
    return a->getInitiative() > b->getInitiative();
});

// Range-based for loop do wykonania ruchów
for (auto& org : organisms) {
    if (org->getLiveLength() > 0) {
        org->move();
        if (org->getLiveLength() > 0) org->action();
    }
}

// Słynny Erase-Remove Idiom - sprzątanie martwych po turze
organisms.erase(
    std::remove_if(organisms.begin(), organisms.end(),
        [](const unique_ptr<Organism>& org) { return org->getLiveLength() <= 0; }),
    organisms.end()
);
```

---

## 2. Abstrakcje i Mechaniki Główne

### `Organism.h` / `Organism.cpp`

- **Rola:** Klasa bazowa (abstrakcyjna) dla każdej żywej i rosnącej istoty na mapie.
- **Co tu się dzieje?**
  Przechowuje statystyki i wdraża **Regułę Pięciu (Rule of Five)**, chroniącą naszą listę _Historii Przodków_ przed wyciekiem podczas klonowania.

**Przykład z kodu:**

```cpp
// Lista przodków z Uniform Initialization
list<pair<int, int>> ancestorHistory{};

// Elementy Reguły Pięciu zapobiegające płytkiemu kopiowaniu (Shallow Copy)
Organism(const Organism& other);
Organism(Organism&& other) noexcept;
auto operator=(const Organism& other) -> Organism&;
auto operator=(Organism&& other) noexcept -> Organism&;
virtual ~Organism() = default;
```

### `Position.h` / `Position.cpp`

- **Rola:** Prosta klasa pomocnicza.
- **Co tu się dzieje?**
  Opakowuje zwykłe współrzędne `x` i `y`. Doskonale na niej widać wdrożony dookoła konwencję **Trailing Return Types** (Strzałkowa deklaracja typów).

**Przykład z kodu:**

```cpp
// Tradycyjnie: void setX(int x);
// Modern C++ (Trailing Return Type):
auto setX(int new_x) -> void {
    x = new_x;
}

auto toString() const -> std::string {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}
```

---

## 3. Podział Królestw (Zwierzęta i Rośliny)

### `Animal.h` / `Animal.cpp`

- **Rola:** Zwierzęta przemieszczające się.
- **Co tu się dzieje?** Zwierzę rozgląda się wokół siebie poprzez `getVectorOfPositionsAround`, losuje nową pozycję i tam wchodzi. Jeśli pole jest zajęte, wchodzi w kolizję `consequences()`.

**Przykład z kodu:**

```cpp
auto Animal::move() -> void {
    // Sprawdzenie dostępnych pól i bezpieczne pobranie randomowego wektora
    vector<Position> pomPositions{ world->getVectorOfPositionsAround(position) };
    if (!pomPositions.empty()) {
        int randomIndex{ rand() % int(pomPositions.size()) };
        Position newPosition{ pomPositions[randomIndex] };

        Organism* metOrganism{ world->getOrganismFromPosition(newPosition.getX(), newPosition.getY()) };
        setPosition(newPosition); // Ruch w pamięci

        // Zderzenie (walka)!
        if (metOrganism != nullptr && metOrganism != this) {
            metOrganism->consequences(this);
        }
    }
}
```

### `Plant.h` / `Plant.cpp`

- **Rola:** Statyczne rośliny.
- **Co tu się dzieje?** Rośliny mają proste życie. Nadpisują funkcję `move()` pustym ciałem.

**Przykład z kodu:**

```cpp
auto Plant::move() -> void {
    // Rośliny się nie ruszają.
}
```

---

## 4. Konkretne Gatunki i Polimorfizm

Krótkie pliki nadpisujące statystyki dla polimorfizmu:

- **`Sheep.cpp`**: `initParams() { setPower(3); setInitiative(3); setLiveLength(10); }`
- **`Wolf.cpp`**: Drapieżnik wyższego rzędu z wysoką inicjatywą (5) i siłą (8).

### `Toadstool.h` / `Toadstool.cpp` (Muchomor)

- **Rola:** Unikalna Roślina - "Zabójca".
- **Co tu się dzieje?** Najlepszy przykład polimorfizmu na obronę. Muchomor nadpisuje mechanizm walki `consequences()`, po czym zabija wszystko, co na niego staje.

**Przykład z kodu:**

```cpp
auto Toadstool::consequences(Organism* attackingOrganism) -> void {
    // Muchomor jest zjadany (życie spada do zera),
    // ale atakujący natychmiast ulega zatruciu i również ginie.
    this->setLiveLength(0);
    attackingOrganism->setLiveLength(0);
}
```

---

## 5. Grafika i Renderowanie (GUI)

### `OLCView.cpp`

- **Rola:** Pomost pomiędzy logiką a silnikiem `olcPixelGameEngine`.
- **Co tu się dzieje?** Klasa odświeża się w każdej klatce za pomocą `OnUserUpdate()`. Naciśnięcie `SPACJI` napędza świat. Rysowanie jest banalne na podstawie pobranego znaku zwierzęcia (`getSign()`).

**Przykład z kodu:**

```cpp
auto OLCView::OnUserUpdate(float fElapsedTime) -> bool {
    // Nowa tura pod Spacją
    if (GetKey(olc::Key::SPACE).bPressed) {
        world.makeTurn(); // Napędzenie logiki
    }

    Clear(olc::Pixel(30, 30, 30));

    // Rysowanie planszy na ekranie po XY
    for (int y{ 0 }; y < world.getWorldY(); ++y) {
        for (int x{ 0 }; x < world.getWorldX(); ++x) {
            Organism* org{ world.getOrganismFromPosition(x, y) };
            olc::Pixel color{ olc::Pixel(50, 150, 50) }; // Domyślna Trawa

            if (org) {
                // Kolorowanie w zależności od znaku
                if (org->getSign() == 'W') color = olc::DARK_GREY;
                else if (org->getSign() == 'T') color = olc::RED;
            }

            FillRect(x * cellSize + 1, y * cellSize + 1, cellSize - 2, cellSize - 2, color);
        }
    }
    return true;
}
```
