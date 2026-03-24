#include <iostream>
#include <string>

using namespace std;

int zadanie2();
int zadanie3();
int zadanie4();

int main() {
    // zadanie2();
    // zadanie3();
    zadanie4();

    return 0;
}

int zadanie2() {
    string imie{};
    std::cout << "Podaj swoje imie: ";
    std::cin >> imie;

    std::cout << "Witaj,  " << imie << std::endl;

    return 0;
}

int zadanie3() {
    int liczba{};

    cout << "Podaj liczbe: ";
    cin >> liczba;

    if (liczba == 0) {
        cout << "Liczba jest równa 0";
    } else if (liczba > 0) {
        cout << "Liczba jest dodatnia";
    } else {
        cout << "Liczba jest ujemna";
    }

    return 0;
}

int zadanie4() {
    bool flaga{true};
    int liczba{};

    // while (flaga)
    // {
    //     cin >> liczba;
    //     if (liczba >= 1 && liczba <= 7) {
    //         flaga = false;
    //     } else {
    //         cout << "Wpisałeś niepoprawną liczbę, spróbuj ponownie." << endl;
    //     }
    // }
    cout << "Podaj liczbę z zakresu <1,7>: " << '\n';
    cin >> liczba;

    switch (liczba)
    {
    case 1:
        cout << "Aktualny dzień to poniedziałek" << endl;
        break;
    case 2:
        cout << "Aktualny dzień to wtorek" << '\n';
        break;
    case 3:
        cout << "Aktualny dzień to środa" << '\n';
        break;
    case 4:
        cout << "Aktualny dzień to czwartek" << '\n';
        break;
    case 5:
        cout << "Aktualny dzień to piątek" << '\n';
        break;
    case 6:
        cout << "Aktualny dzień to sobota" << '\n';
        break;
    case 7:
        cout << "Aktualny dzień to niedziela" << '\n';
        break;
    default:
        cout << "Błąd: Podano liczbę spoza zakresu" << '\n';
        break;
    }
}
