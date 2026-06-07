#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Animal
{
      private:
            string name{"unknown"};
      public:
            Animal(string animal_name) : name{animal_name} {
                  cout << "Konstruktor Animal" << endl;
            }

            auto get_name() const -> string {
                  return name;
            }

            auto print() const -> void {
                  cout << "Zwierze: " << name << endl;
            }

            virtual auto make_sound() const -> void {
                  cout << "Nieznany dzwiek" << endl;
            }
};

class Dog : public Animal
{
      private:
            string breed{"unknown"};
      public:
            Dog(string dog_name, string dog_breed) : Animal{dog_name}, breed{dog_breed} {
                  cout << "Konstruktor Dog" << endl;
            }

            auto print_dog() const -> void {
                  cout << "Pies: " << get_name() << ", rasa: " << breed << endl;
            }

            auto make_sound() const -> void override {
                  cout << "Hau!" << endl;
            }
};

class Cat : public Animal 
{
      public:
            Cat(string cat_name) : Animal{cat_name} {
                  cout << "Konstruktor Cat" << endl;            
            }

            auto make_sound() const -> void override {
                  cout << "Miau!" << endl;
            }
};


auto describe_animal(const Animal& animal) -> void {
      cout << "Nazwa: " << animal.get_name() << endl;
      animal.make_sound();
}

auto main() -> int {
      Dog dog{"Reksio", "kundelek"};
      Cat cat{"Mruczek"};

      describe_animal(dog);
      describe_animal(cat);
      
      return 0;
}