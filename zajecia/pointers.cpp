#include <iostream>
#include <memory>
#include <string>

using namespace std;

class Book
{
      private:
            string title{"unknown"};
      public:
            Book(string book_title) : title{book_title} {
                  cout << "Konstruktor Book" << endl;
            }

            ~Book() {
                  cout << "Destruktor Book" << endl;
            }
            auto print() const -> void
            {
                  cout << "Book: " << title << endl;
            }

            
};

auto add_one(int& value) -> void
{
      value += 1;
}

auto show_book(const Book& book) -> void
{
      book.print();
}

class Counter
{
      private:
            int value{0};
      
      public:
            auto increment() -> void {
                  value += 1;
            }

            auto print() const -> void {
                  cout << "value = " << value << endl;
            }
};

auto main() -> int
{
      int x{10};
      int& ref{x}; // referencja (druga nazwa zmiennej)


      cout << "wartosc x: " << x << endl;
      cout << "adres x: " << &x << endl;

      // referencja
      cout << ref << endl;
      ref = 25;
      cout << x << endl;
      cout << ref << endl;

      // referencja w argumentach funkcji
      int number{10};
      add_one(number);
      cout << number << endl;

      // funkcja dostaje dostep do istniejacego obiektu, ale nie powinna go zmieniac.
      Book book{"C++"};
      show_book(book);

      // wskaznik jako zmienna przechowujaca adres
      int a{10};
      int* p{&x};

      cout << "a = " << x << endl;
      cout << "adres x = " << &x << endl;
      cout << "p = " << p << endl;

      // Dereferencja
      int b{10};
      int* c{&b};
      cout << *c << endl;
      *c = 99;
      cout << b << endl;

      // Pusty wskaźnik nullptr
      int* d{nullptr};

      if (d != nullptr)
      {
            cout << *d << endl;
      } else {
            cout << "Wskaznik jest pusty" << endl;
      }

      // Wskaźnik do obiektu i operator ->
      Counter counter{};
      Counter* ptr{&counter};

      counter.increment();
      counter.print();

      ptr->increment();
      ptr->print();

      // Unique pointer (unique_ptr)
      unique_ptr<Book> book1{make_unique<Book>("Nowoczesny C++")};
      book1->print();


      return 0;
}