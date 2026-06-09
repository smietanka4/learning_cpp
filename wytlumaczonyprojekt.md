# Przewodnik Przez Kod (Tłumaczenie Linijka po Linijce "Dla Laika")

Ten dokument to zbiór pełnych fragmentów naszego kodu źródłowego, z umieszczonymi w środku, ponumerowanymi i prostymi komentarzami (krok po kroku), tłumaczącymi działanie całego ekosystemu.

---

### 1. Włącznik Prądu (`LivingWorld.cpp` - funkcja `main`)

To pierwsza rzecz, która odpala się w każdym programie C++. Wyobraź sobie, że to przycisk START.

```cpp
auto main() -> int
{
	// 1. Odpalamy losowość w komputerze, żeby każde uruchomienie gry miało inny układ
	srand(time(nullptr));

	// 2. Ustawiamy rozmiar naszej szachownicy
	int worldSizeX{ 20 };
	int worldSizeY{ 20 };

	// 3. Tworzymy obiekt naszej planszy (Menedżera Gry)
	World world(worldSizeX, worldSizeY);

	// 4. Wrzucamy zwierzęta na planszę.
	// `make_unique` to menedżer, który opiekuje się pamięcią zwierzęcia w C++.
	// W nawiasie budujemy nową Owcę, dajemy jej pozycję X=1, Y=1 i pokazujemy jej "tu jest twój świat".
	world.addOrganism(std::make_unique<Sheep>(Position(1, 1), &world));
	world.addOrganism(std::make_unique<Wolf>(Position(5, 5), &world));
	world.addOrganism(std::make_unique<Toadstool>(Position(3, 3), &world));

	// 5. Powołujemy do życia kamerę - program do rysowania grafiki (silnik OLC)
	int cellSize{ 20 };
	OLCView view(world, cellSize);

	// 6. Uruchamiamy okienko z grą i oddajemy mu kontrolę!
	if (view.Construct(worldSizeX * cellSize, worldSizeY * cellSize, 2, 2)) {
		view.Start();
	}

	// 7. Zakończenie programu (nigdy tu nie dojdzie, dopóki nie zamkniesz okienka)
	return 0;
}
```

---

### 2. Menedżer Gry (`World.cpp` - funkcja `makeTurn`)

Główny silnik gry. To on decyduje, co dzieje się, gdy klikasz spację.

```cpp
auto World::makeTurn() -> void
{
	// 1. Zanim ktokolwiek się ruszy, sortujemy wszystkich "graczy" w koszyku.
	// Kto ma największą Inicjatywę (szybkość), będzie na początku kolejki.
	std::sort(organisms.begin(), organisms.end(), [](const unique_ptr<Organism>& a, const unique_ptr<Organism>& b) {
		return a->getInitiative() > b->getInitiative();
	});

	// 2. Lecimy w pętli przez każde zwierzę/roślinę z naszej posortowanej kolejki.
	for (auto& org : organisms) {

		// 3. Jeśli ten ktoś jeszcze żyje (bo mógł zostać zjedzony sekundę wcześniej przez szybszego wilka!)...
		if (org->getLiveLength() > 0) {
			org->move(); // Zrób swój ruch na planszy

			// 4. Jeśli po ruchu (i ewentualnej walce z wrogiem!) nadal żyjesz...
			if (org->getLiveLength() > 0) {
				org->action(); // Rozmóż się / zasiej ziarna.
			}
		}
	}

	// 5. Skoro cała tura dobiegła końca, wszyscy starzeją się o 1 rok.
	for (auto& org : organisms) {
		if (org->getLiveLength() > 0) {
			// Zmniejsz hp (czas do śmierci ze starości) i dodaj trochę siły za bycie starszym
			org->setLiveLength(org->getLiveLength() - 1);
			org->setPower(org->getPower() + 1);
		}
	}

	// 6. Sprzątanie ciał (zjawisko Erase-Remove). Bierzemy listę "organisms",
	// wyłapujemy każdego, kto ma żywotność <= 0 i wyrzucamy jego zwłoki z pamięci programu.
	organisms.erase(
		std::remove_if(organisms.begin(), organisms.end(),
			[](const unique_ptr<Organism>& org) { return org->getLiveLength() <= 0; }),
		organisms.end()
	);

	// 7. W punkcie 4. mogły urodzić się małe owieczki (wpadły do "poczekalni" newOrganisms).
	// Przerzucamy je teraz z "poczekalni" na główną mapę!
	for (auto& newOrg : newOrganisms) {
		organisms.push_back(std::move(newOrg));
	}
	newOrganisms.clear(); // Czyścimy poczekalnię.

	// 8. Tura 1 zmienia się w Turę 2. Koniec!
	turn++;
}
```

---

### 3. Ruch Zwierząt (`Animal.cpp` - funkcja `move`)

Zwierzęta mają nogi, więc skanują teren i decydują, dokąd pójść.

```cpp
auto Animal::move() -> void
{
	// 1. Hej Świecie, daj mi listę wszystkich pól dookoła mnie (góra, dół, lewo, prawo, na ukos).
	// Dostaję worek współrzędnych "pomPositions".
	vector<Position> pomPositions{ world->getVectorOfPositionsAround(position) };

	// 2. Jeśli ten worek nie jest pusty (bo np. nie jestem zablokowany w rogu mapy)
	if (!pomPositions.empty()) {

		// 3. Wylosuj jedną liczbę ze środka tego worka (np. 3)
		int randomIndex{ rand() % int(pomPositions.size()) };

		// 4. "newPosition" to będzie to nowo wylosowane pole docelowe, na które zaraz wskoczę.
		Position newPosition{ pomPositions[randomIndex] };

		// 5. Zapisuję do mojego pamiętnika, gdzie stałem krok wcześniej
		setLastPosition(position);

		// 6. Mówię do Świata: "Hej, czy na polu nowym polu na które idę, ktoś już przypadkiem stoi?"
		Organism* metOrganism{ world->getOrganismFromPosition(newPosition.getX(), newPosition.getY()) };

		// 7. Dobra, przestawiam wreszcie siebie na to wylosowane miejsce
		setPosition(newPosition);

		// 8. ZDERZENIE: Jeśli na tym polu rzeczywiście ktoś stał i ten ktoś to nie jestem ja sam...
		if (metOrganism != nullptr && metOrganism != this) {
			// ...to krzyczę do tego kogoś: Uruchom na mnie funkcję CONSEQUENCES (walkę)!
			metOrganism->consequences(this);
		}
	}
}
```

---

### 4. Formularz Życia i Kolizja (`Organism.cpp` - funkcja `consequences`)

Skoro zwierzę przed chwilą uderzyło w inne, jak decydujemy kto ginie?

```cpp
auto Organism::consequences(Organism* attackingOrganism) -> void
{
	// 1. Sprawdzam kto jest silniejszy.
	// Jeśli ja (ten, na którego właśnie wepchnięto się w move()) mam większą siłę...
	if (this->getPower() > attackingOrganism->getPower()) {
		// 2. ...to intruz odpada. Ustawiam jego pozostały czas życia natychmiast na 0.
		attackingOrganism->setLiveLength(0);
	}
	else {
		// 3. W przeciwnym razie intruz był ode mnie silniejszy. To ja ginę.
		this->setLiveLength(0);
	}
}
```

---

### 5. Muchomor Oszukuje Grę (`Toadstool.cpp` - funkcja `consequences`)

Muchomor (trująca roślina) wie, że z wilkiem przegra z powodu małej siły. Więc pisze dla siebie własną, inną regułę kolizji:

```cpp
auto Toadstool::consequences(Organism* attackingOrganism) -> void
{
	// 1. Ignoruję zasady "kto jest silniejszy" z klasy Organism.
	// Jako Muchomor zostaję zjedzony i zdeptany (zmieniam sobie życie na 0).
	this->setLiveLength(0);

	// 2. ALE intruz, który mnie właśnie zjadł, natychmiast się truje!
	// Również jemu zmieniam życie na 0.
	attackingOrganism->setLiveLength(0);

	// Efekt: Znikam ja i znika potężny wilk. Zabieram go ze sobą do grobu.
}
```

---

### 6. Ruch i Akcja Roślin (`Plant.cpp` - funkcja `move` oraz `action`)

Dlaczego oddzielono rośliny od zwierząt?

```cpp
auto Plant::move() -> void
{
	// 1. Rośliny po prostu rosną w ziemi, a nie mają nóg.
	// Dlatego w przeciwieństwie do zwierząt z Animal.cpp, funkcja ruchu jest kompletnie pusta.
}

auto Plant::action() -> void
{
	// 1. Co mogą więc robić rośliny? Rosną i zasiewają się, jeśli mają na to siłę reprodukcyjną
	if (ifReproduce()) {

		// 2. Zamiast szukać jakichkolwiek pól obok, roślina woła do Świata:
		// "Daj mi wektor TYLKO tych całkowicie wolnych pól z ziemią na mapie".
		vector<Position> pomPositions{ world->getVectorOfFreePositionsAround(position) };

		// 3. Jeśli jakiekolwiek pole z ziemią jest obok...
		if (!pomPositions.empty()) {
			int randomIndex{ rand() % int(pomPositions.size()) };
			Position newPosition{ pomPositions[randomIndex] };

			// 4. Klonuję siebie (z Owcy rodzi się kolejna Owca, z Rośliny rodzi się roślina)
			Organism* newPlant{ this->clone() };

			// 5. Ustawiam temu dzieciakowi wszystkie startowe wartości i kładę go na tym polu
			newPlant->initParams();
			newPlant->setPosition(newPosition);

			// 6. Oddaję dziecko pod nadzór Świata (wpadnie na mapę w kolejnej turze)
			world->addNewOrganism(unique_ptr<Organism>(newPlant));
		}
	}
}
```

---

### 7. Poszczególne Zwierzaki (`Sheep.cpp`, `Wolf.cpp`, `Dandelion.cpp`)

Skoro `Animal.cpp` umie chodzić, a `Plant.cpp` siać nasiona, konkretne pliki służą do tylko jednej prostej rzeczy - dają na start konkretne statystyki. Nie trzeba w nich od nowa kodować logiki ruchu.

```cpp
// Wilk to król dżungli
auto Wolf::initParams() -> void {
	this->setPower(8);       // 1. Ogromna siła miażdżąca na start
	this->setInitiative(5);  // 2. Duża szybkość, ruszy się zazwyczaj jako jeden z pierwszych w turze
	this->setLiveLength(20); // 3. Może dożyć aż 20 tur!
	this->powerToReproduce = 16;
}

// Owca to zaledwie zwykłe danie
auto Sheep::initParams() -> void {
	this->setPower(3);       // 1. Mało siły...
	this->setInitiative(3);  // 2. Słabszy refleks...
	this->setLiveLength(10); // 3. Dożyje najwyżej do dziesiątki.
	this->powerToReproduce = 6;
}

// Mlecz ma jeszcze gorzej!
auto Dandelion::initParams() -> void {
	this->setPower(0);       // 1. Zero siły, absolutnie każdy jest w stanie go zjeść.
	this->setInitiative(0);  // 2. Zerowy refleks, roślina jest na samym końcu tury w `makeTurn()`.
	this->setLiveLength(5);
	this->powerToReproduce = 2; // 3. Łatwo i bardzo tanio się rozsiewa.
}
```

---

### 8. Pozycja (`Position.h`)

Plik-Woreczek. Jak kogoś pytać po co to stworzono?

```cpp
class Position
{
private:
	// 1. Dwie absolutnie najważniejsze dane: Współrzędna X i Współrzędna Y
	int x{};
	int y{};

public:
	// 2. Zamiast w każdym pliku musieć nosić dwie zmienne obok siebie, stworzyliśmy obiekt.
	// Pozwala on łatwo odpytywać planszę: getX(), getY().
	Position(int x, int y);
	auto getX() const -> int;
	auto setX(int x) -> void;
	//...
};
```

---

### 9. Zapis Gry (`World.cpp` - fragment funkcji zapisującej plik)

Gdy gracz zdecyduje się zamrozić grę i zrzucić ją na dysk twardy komputera.

```cpp
auto World::writeWorld(string fileName) -> void
{
	// 1. Bierzemy ścieżkę do pliku tekstowego / binarnego podaną w nazwie i otwieramy go
	std::ofstream my_file(fileName, std::ios::binary);

	if (my_file.is_open()) {
		// 2. Zapisujemy w pliku wymiary X oraz Y naszej planszy
		my_file.write((char*)&worldX, sizeof(int));
		my_file.write((char*)&worldY, sizeof(int));

		// 3. Zapisujemy obecny numer tury, żeby wiedzieć na którym kroku stanęła gra
		my_file.write((char*)&turn, sizeof(int));

		// 4. Zapisujemy liczbę wszystkich żywych zwierząt aktualnie chodzących po planszy
		int orgSize{ int(organisms.size()) };
		my_file.write((char*)&orgSize, sizeof(int));

		// 5. Lecimy pętlą po każdym pojedynczym zwierzaczku...
		for (auto& org : organisms) {
			// 6. ... i wydajemy polecenie: "Podyktuj mi teraz swoje statystyki wprost do pliku!"
			org->serialize(my_file);
		}

		// 7. Zamykamy słoik (zwalniamy plik)
		my_file.close();
	}
}
```

---

### 10. Kamera Gry (`OLCView.cpp` - funkcja `OnUserUpdate`)

Odświeżanie okna, które patrzy na nasz Świat. Uruchamia się w kółko kilkadziesiąt razy na sekundę.

```cpp
auto OLCView::OnUserUpdate(float fElapsedTime) -> bool
{
	// 1. Sprawdzam, czy gracz nacisnął w okienku Spację na klawiaturze
	if (GetKey(olc::Key::SPACE).bPressed) {
		world.makeTurn(); // Jeśli tak, wołam Menedżera: "Zrób nową turę z World.cpp!"
	}

	// 2. Czyszczę całkowicie ekran na czarny kolor (jak ścieranie tablicy w szkole)
	Clear(olc::Pixel(30, 30, 30));

	// 3. Przelatuję pętlami po każdym rzędzie (Y) i po każdej kolumnie (X) na naszej planszy...
	for (int y{ 0 }; y < world.getWorldY(); ++y) {
		for (int x{ 0 }; x < world.getWorldX(); ++x) {

			// 4. Biorę organizm stojący na danej kratce
			Organism* org{ world.getOrganismFromPosition(x, y) };

			// 5. Wybieram domyślny zielony kolor (bo jak nikogo nie ma, to znaczy że jest trawa)
			olc::Pixel color{ olc::Pixel(50, 150, 50) };

			// 6. Ale jeśli ktoś na tej trawie jednak stoi!
			if (org) {
				char sign{ org->getSign() };

				// Zmieniam farbę z zielonej na inną, w zależności od znaczka:
				if (sign == 'S') color = olc::WHITE;           // Owca = biała
				else if (sign == 'W') color = olc::DARK_GREY;  // Wilk = ciemnoszary
				else if (sign == 'D') color = olc::YELLOW;     // Mlecz = żółty
				else if (sign == 'T') color = olc::RED;        // Muchomor = czerwony
				else color = olc::MAGENTA;                     // Niezidentyfikowany organizm
			}

			// 7. Na samym końcu maluję kwadracik o rozmiarach klatki (cellSize) tą wybraną przed chwilą farbą!
			FillRect(x * cellSize + 1, y * cellSize + 1, cellSize - 2, cellSize - 2, color);
		}
	}

	return true;
}
```
