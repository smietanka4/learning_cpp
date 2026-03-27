#include <iostream>
#include <string>

using namespace std;

int zadanie2();
int zadanie3();
int zadanie4();
int zadanie5();
int zadanie6();
int zadanie7();
int zadanie8();
int zadanie9();

int main() {
    // zadanie2();
    // zadanie3();
    // zadanie4();
    // zadanie5();
    // zadanie6();
    // zadanie7();
    // zadanie8();
    zadanie9();

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

    return 0;
}

int zadanie5() {
    int n{};
    int suma{0};

    cout << "Podaj liczbę: " << '\n';
    cin >> n;
    
    for (int i = 0; i <= n; i++) {
        suma += i;
    }

    cout << "Suma jest równa: " << suma << '\n';

    return 0;
}

int zadanie6() {
    string haslo{};
    string correct_haslo{"cpp123"};

    while (haslo != correct_haslo);
        cout << "Wprowadź hasło: " << '\n';
        cin >> haslo;
        if (haslo != correct_haslo) {
            cout << "Niepoprawne hasło, spróbuj ponownie" << '\n';
        } else {
            cout << "Dostęp przyznany." << '\n';
        }

        return 0;
}

int zadanie7() {
    string slowo{};
    cout << "Podaj słowo: " << '\n';
    cin >> slowo;
    cout << "Długość podanego słowa: " << slowo.length() << '\n';

    return 0;
}

int zadanie8() {
    string slowo{};
    cout << "Podaj słowo: " << '\n';
    cin >> slowo;

    if (slowo.length() != 0) {
        cout << "Pierwsza literka podanego słowa: " << slowo[0] << "\nOstatnia literka podanego słowa: " << slowo[slowo.length()-1] << '\n';
    }

    return 0;
}

int zadanie9() {
    string slowo{};
    cout << "Podaj słowo, które chcesz sprawdzić czy jest palindromem: " << '\n';
    cin >> slowo;

    bool isPalindrome{true};

    for (int i = 0; i <= slowo.length() / 2; i++) {
        if (slowo[i] != slowo[slowo.length() - 1 - i]) {
            isPalindrome = false;
            cout << "Słowo nie jest palindromem" << '\n';
            break;
        } else {
            cout << slowo[i] << '=' << slowo[slowo.length() - i - 1] << '\n';
        }
    }

    if (isPalindrome) {
            cout << "Słowo jest palindromem" << '\n';
    }

    return 0;
}