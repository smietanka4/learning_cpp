#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Shape
{
      public:
            virtual auto area() const -> double = 0;
            virtual auto print() const -> void = 0;
            virtual ~Shape() = default;
};

class Rectangle : public Shape
{
      private:
            double width{1.0};
            double height{1.0};
      public:
            Rectangle(double rectangle_width, double rectangle_height) : width{rectangle_width}, height{rectangle_height} {}

            auto area() const -> double override {
                return width * height;  
            }

            auto print() const -> void override {
                  cout << "Prostokat: " << width << " x " << height << endl;
            }
};

auto print_shape(const Shape& shape) -> void 
{
      shape.print();
      cout << "Pole: " << shape.area() << endl;
}

auto main() -> int {

      return 0;
}