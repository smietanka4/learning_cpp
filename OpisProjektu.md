# Projekt Świat - Symulacja (C++17 / Modern C++)

Projekt ten stanowi niezwykle obszerną realizację zadania polegającego na rozbudowie płaskiego, dwuwymiarowego świata wirtualnego, składającego się z różnorodnych gatunków fauny i flory. Baza została stworzona na twardych fundamentach **modelu obiektowego**, ścisłych założeń **SOLID** oraz najświeższych standardów z **Modern C++**.

## 1. Geneza i Ewolucja z repozytorium PythonWorld (World01 - World04)

Historia projektu rozpoczęła się od pobrania wzorców wdrożeniowych z repozytorium instruktorskiego `PythonWorld`. Struktura, która pierwotnie była dynamiczna i słabo typowana w języku Python, musiała zostać poddana twardej konwersji obiektowej na silnie typowany język C++:

- **World01 (Podstawy Reprezentacji C++)**: Z pierwszego etapu zaadaptowano główną ideę szkieletu gry: wyodrębniono zarządzającą klasę `World` oraz nadrzędną klasę encji `Organism`. Stworzono cykl turowy oparty na sekwencyjnym wywoływaniu metod `move()` oraz `action()`. Po raz pierwszy narzucono organizmom pozycjonowanie na siatce X/Y (klasa `Position`).
- **World02 (Struktura Klas, Abstrakcja i Dziedziczenie)**: Ukształtowano twardy podział ról na rośliny (`Plant`) i zwierzęta (`Animal`). Utworzono hierarchię klas: bazowy `Organism` stał się w dużej mierze klasą abstrakcyjną, wymuszającą na podklasach nadpisanie wirtualnych funkcji (`override`). Rośliny utraciły możliwość przemieszczania się (`move() { }`), skupiając się na rozsiewaniu nasion, natomiast Zwierzęta uzyskały zaawansowany radar do poszukiwania pustych pól na mapie.
- **World03 (Specyfika Gatunkowa)**: Od tego momentu organizmy przestały być "szarymi klockami". Zaimplementowano dedykowane parametry witalne i bojowe (Siła, Inicjatywa, Długość Życia, Siła potrzebna do rozmnażania). Zaprojektowano konkretne klasy (np. **Owca / Sheep**, **Mlecz / Dandelion**, **Wilk / Wolf**), które same nadpisują swoje parametry startowe wywołując `initParams()`.
- **World04 (Specjalne Mechaniki Kolizji i Interfejsy)**: Najwyższy stopień abstrakcji; wprowadzono specjalne zachowania na styku dwóch stykających się encji. Zaimplementowano skrajnie śmiercionośnego **Muchomora (Toadstool)**. Nadpisana z klasy bazowej funkcja `consequences(Organism* attackingOrganism)` sprawia, że muchomor potrafi odwrócić zasady walki — po zjedzeniu niszczy potężniejszego od siebie przeciwnika, nawet takiego jak Wilk.

## 2. Refaktoryzacja Szablonu i Naprawa Standardów

Zastany na początku prac szablon `projekt-wiat-smietanka4` zawierał pewne luki logiczne, które w C++ prowadziłyby do katastrofy lub przynajmniej do rażącego łamania zasad programowania zorientowanego obiektowo. Oto najważniejsze wprowadzone przez nas naprawy od momentu "Zera":

### A. Wdrożenie Prawdziwego Polimorfizmu (`std::unique_ptr`)

Pierwotny szablon wektora w C++ próbował przechowywać obiekty poprzez wartość (`std::vector<Organism> organisms`). Prowadziło to do obcięcia typów potomnych (tzw. zjawisko _Object Slicing_), przez co funkcje wirtualne, mimo że istniały, nie mogły zadziałać. Zmieniono wektor na kontener inteligentnych wskaźników: `std::vector<std::unique_ptr<Organism>>`. To wyzwoliło polimorfizm — teraz wywołując w pętli `org->move()`, C++ automatycznie wie, czy ma poruszyć Wilka, czy Muchomora. Pamięć czyści się samoistnie (bez ryzykownych wywołań `delete`).

### B. Historia Rodowodu i The Rule of Five (Reguła Pięciu)

Dodano zaawansowaną listę historii życiowej organizmu (tury urodzenia i śmierci), co spowodowało, że domyślne konstruktory kopiujące generowane przez kompilator mogły współdzielić wskaźniki do zasobów starych obiektów. By temu zapobiec i stworzyć niezawodny klonator gatunków (`clone()`), do `Organism` dołączono w pełni własnoręcznie zdefiniowaną **Regułę Pięciu**:

1. Konstruktor kopiujący (`Organism(const Organism& other)`)
2. Konstruktor przenoszący (`Organism(Organism&& other) noexcept`)
3. Operator przypisania kopiującego
4. Operator przypisania przenoszącego
5. Wirtualny destruktor `virtual ~Organism() = default;`

### C. Zgodność z S.O.L.I.D. (Delegacja Serializacji)

Zamiast ładować ogromną drabinkę warunków `if` do pliku świata `World` w celu zapisu wszystkich organizmów na dysk (`Single Responsibility Principle`), to każda jednostka na świecie została wyposażona we własne, samodzielne mechanizmy wirtualne `serialize(std::ofstream& out)`. Dzięki temu klasa Świat w ogóle nie interesuje się detalami budowy zwierzęcia; Świat jedynie otwiera strumień pliku binarego i prosi swoje organizmy: "niech każdy zapisze się sam".

## 3. Przebudowa Front-endu: GUI olcPixelGameEngine

Początkowo rozważano wdrożenie złożonego interfejsu przy pomocy **SFML** z wykorzystaniem renderowania obiektowego. Niosło to ze sobą gigantyczne obciążenia techniczne na środowiskach (zwłaszcza konfigurowanie DLL i pakietów CMake pod środowisko Windows/MinGW).
Ostatecznie zaimplementowano niesamowicie lekki i zaawansowany silnik okienkowy z pliku nagłówkowego **olcPixelGameEngine**. Wszystkie skomplikowane zdarzenia okienkowe zamknięto w małym interfejsie klasy `OLCView`. Pozwoliło to na idealne i płynne rysowanie graficznej reprezentacji siatki organizmów na ekranie za pomocą zaledwie paru wierszy kodu i natywnych bibliotek środowiska Windows. W locie rozwiązywaliśmy tam m.in. zderzenie nazw definicji typu `std::byte` z bibliotek w Windows.h, przenosząc wywołania przestrzeni nazw do wnętrza plików konfiguracyjnych.

## 4. Totalna Modernizacja (Styl: Modern C++)

Pod sam koniec całego cyklu ulepszeń, kod źródłowy został zrefaktoryzowany z tradycyjnego stylu C++ wprost do wymagań z podesłanych plików laboratoriów C++ (`klasy.cpp` i `zaj01.cpp`):

- **Trailing Return Types (Składnia Strzałkowa z Auto)**: Każda definicja funkcji we wszystkich klasach zyskała innowacyjną konstrukcję, np. `auto getPower() const -> int;`. Sprawia to, że formatowanie pionowe i horyzontalne metod oraz konstruktorów stało się nieporównywalnie czystsze w listingu plików `.cpp`.
- **Uniform Initialization (Inicjalizacja Klamrowa)**: Aby drastycznie zwiększyć poprawność typowania i ochronić program przed "Zwężeniami Zmiennych" (Narrowing Conversions), tradycyjny znak równej asygnacji (`=`) zastąpiono nowoczesnym podejściem inicjalizacji wewnątrz klamer `int x{};`. Co więcej, zlikwidowano również odwołania do uciążliwych i starych typowań `static_cast` na rzecz bezpiecznej konwersji używającej naturalnej struktury wywołań np. `int(v.size())`.

## 5. Jak Uruchomić Gotowe Środowisko?

Kompilacja wykorzystuje potęgę nowego standardu C++17 z załączeniem flag bibliotek okienkowych pod architekturę Windows. Otwórz swój terminal w projekcie i zbuduj silnik poleceniem kompilatora:

```bash
g++ -std=c++17 LivingWorld.cpp Organism.cpp World.cpp Position.cpp Animal.cpp Plant.cpp Sheep.cpp Dandelion.cpp Wolf.cpp Toadstool.cpp OLCView.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -o LivingWorld.exe
```

Gdy plik zostanie wygenerowany z sukcesem (zero błędów logiki dzięki naszej pracy na standardach), wykonaj komendę:

```bash
.\LivingWorld.exe
```

W otwartym oknie graficznym naciśnij **Spację**, aby wywołać symulację do następnej tury! Pamiętaj, każda Spacja to jedna jednostka "cyklu życia", w którym rozegrają się walki wilków, śmiertelne posiłki u muchomorów i wiekowe procesy rozrostu dmuchawców na zielonej trawie.
