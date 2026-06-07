#include <iostream>
#include <string>
#include <utility>
#include <vector>

using std::cout;
using std::endl;
using std::move;
using std::string;
using std::vector;


class Student
{
public:
    string name{};
    int semester{1};
    
    auto print() const -> void
    {
        cout << "Student: " << name << ", semestr: " << semester << endl;
    }
};

class Example
{
public:
    // Dostepne z zewnatrz klasy
    auto public_method() -> void {};

private:
    // Dostepne tylko wewnatrz tej klasy
    int secret_value{42};
protected:
    // Dostepne wenatrz tej klasy i w klasach dziedziczacych
    int value_for_children{10};
};

class Rectangle
{
private:
    double width{1.0};
    double height{1.0};

public:
    auto area() const -> double
    {
        return width * height;
    }
    
    auto print() const -> void
    {
        cout << "width = " << width
            << ", height = " << height
            << ", area = " << area() << endl;
    }

};

class BankAccount
{
    private:
        string owner{"unknown"};
        double balance{0.0};
    
        public:
            auto set_owner(string new_owner) -> void
            {
                if (!new_owner.empty())
                {
                    owner = new_owner;
                }
            }

            auto get_owner() const -> string
            {
                return owner;
            }

            auto get_balance() const -> double
            {
                return balance;
            }

            auto deposit(double amount) -> void
            {
                if (amount > 0.0)
                {
                    balance += amount;
                }
            }

            auto withdraw(double amount) -> bool
            {
                if (amount > 0.0 && amount <= balance) {
                    balance -= amount;
                    return true;
                }

                return false;
            }
};


// Konstruktor i lista inicjalizacyjna
class Course
{
    private:
        string name{"unknown"};
        int ects{0};
    
    public:
        Course(string course_name, int course_ects) 
            : name{course_name}, ects{course_ects} {
                cout << "Konstruktor Course" << endl;
            }

        auto print() const -> void
        {
            cout << name << " (ECTS: " << ects << ")" << endl;
        }
};

// Kopiowanie, przenoszenie, przypisanie i destruktor
class Note
{
    private:
        string title{"untitled"};
        vector<string> lines{};

    public:
        Note()
        {
            cout << "Konstruktor domyslny" << endl;
        }

        Note(string note_title)
            : title{move(note_title)}
        {
            cout << "Konstruktor z argumentem" << endl;
        }

        Note(Note& other)
            : title(other.title), lines{other.lines} {
                cout << "Konstruktor kopiujacy" << endl;
            }

        Note(Note&& other) noexcept
            : title{move(other.title)}, lines{move(other.lines)} {
                cout << "Konstruktor przenoszący" << endl;
            }

        auto operator=(const Note& other) -> Note&
        {
            cout << "Kopiujacy operator przypisania" << endl;

            if (this != &other)
            {
                title = other.title;
                lines = other.lines;
            }

            return *this;
        }

        auto operator=(Note&& other) noexcept -> Note&
        {
            cout << "Przenoszacy operator przypisania" << endl;

            if (this != &other)
            {
                title = move(other.title);
                lines = move(other.lines);
            }

            return *this;
        }

        ~Note() {
            cout << "Destruktor: " << title << endl;
        }

        auto add_line(string line) -> void
        {
            lines.push_back(move(line));
        }

        auto print() const -> void
        {
            cout << "Notatka: " << title << endl;
            for (const auto& line : lines)
            {
                cout << "- " << line << endl;
            }
        }
};

auto create_note() -> Note
{
    Note temp{"Tymczasowa"};
    temp.add_line("Ta notatka powstala w funkcji");
    return temp;
}

auto main() -> int
{
    Student s{};
    s.name = "Anna";
    s.semester = 2;
    s.print(); 

    Rectangle r{};
    r.print();

    BankAccount bank{};
    bank.deposit(500);
    bank.withdraw(300);
    cout << bank.get_balance() << endl;

    Course cpp{"Programowanie w C++", 5};
    cpp.print();

    cout << "===============" << endl;

    cout << "--- Tworzenie obiektu ---" << endl;
    Note a{"C++"};
    a.add_line("Klasy moga miec pola i metody");

    cout << "--- Kopiowanie przy tworzeniu ---" << endl;
    Note b{a};

    cout << "--- Kopiujace przypisanie ---" << endl;
    Note c{"Pusta"};
    c = a;

    cout << "--- Przenoszenie przy tworzeniu ---" << endl;
    Note d{move(a)};

    cout << "--- Przenoszace przypisanie ---" << endl;
    Note e{"Inna"};
    e = create_note();

    cout << "--- Koniec programu ---" << endl;
    return 0;
}