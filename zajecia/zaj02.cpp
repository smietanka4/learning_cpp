#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Book
{
    private:
        string title{};
        string author{};
        int year{};

    public:
        auto set_title(string new_title) -> void {
            if (!new_title.empty()) {
                title = new_title;
            }
        }

        auto get_title() -> string {
            return title;
        }

        auto set_author(string new_author) -> void {
            if (!new_author.empty()) {
                author = new_author;
            }
        }

        auto get_author() -> string {
            return author;
        }

        auto set_year(int new_year) -> void {
            if (new_year > 0) {
                year = new_year;
            }
        }

        auto get_year() -> int {
            return year;
        }


        Book(string title, string author, int year) : title{title}, author{author}, year{year} {
            cout << "Konstruktor Book" << endl;
        };

        auto print() const -> void {
            cout << "Title " << title << ", Author " << author << ", year " << year << endl;
        }

};

class Movie
{
    private:
        string title{"unknown"};
        int duration_minutes{0};
        double rating{0.0};

    public:
        auto print() const -> void {
            cout << "Movie: " << title << ", Duration: " << duration_minutes << ", Rating: " << rating << endl;
        }

        auto set_title(string new_title) -> void {
            title = new_title;
        }

        auto set_duration(int new_duration) -> void {
            duration_minutes = new_duration;
        }

        auto set_rating(double new_rating) -> void {
            rating = new_rating;
        }

        auto get_title() -> string {
            return title;
        }

        auto get_duration() -> int {
            return duration_minutes;
        }

        auto get_rating() -> double {
            return rating;
        }
};

class Product
{
    private:
        string name{};
        double price{};
        int quantity{};

    public:
        Product(string name, double price, int quantity) : name{name}, price{price}, quantity{quantity} {
            cout << "Konstruktor Product" << endl;
        };

        auto print() const -> void {
            cout << name << price << quantity << endl;
        }

        auto total_value() const -> double {
            return price*quantity;
        }
};

class Task 
{
    private:
        string name{};
        bool done{false};

    public:
        Task(string name) : name{name} {};

        Task(Task& other) : name{other.name} {
            cout<<"Konstruktor kopiujacy Task" << endl;
        };

        auto operator=(const Task& other) -> Task& {
            cout << "Kopiujacy operator przypisania Task" << endl;

            if (this != &other) {
                name = other.name;
                done = other.done;
            }

            return *this;
        }

        Task(Task&& other) noexcept : name{move(other.name)}, done{other.done} {
            cout << "Konstruktor przenoszacy Task" << endl;
        }

        auto operator=(Task&& other) noexcept -> Task& {
            cout << "Przenoszacy operator przypisania Task" << endl;

            if (this != &other) {
                name = move(other.name);
                done = move(other.done);
            }

            return *this;
        }

        ~Task() {
            cout << "Destruktor Task: " << name << endl;
        }

        auto mark_done() -> void {
            done = true;
        }

        auto print() const -> void {
            cout << name << done << endl;
        }
};

class Playlist
{
    private:
        string name{};
        vector<string> songs{};

    public:
        Playlist(string name) : name{name} {};

        auto add_song(string song) -> void {
            songs.push_back(move(song));
        }

        Playlist(Playlist& other) : name{other.name}, songs{other.songs} {
            cout << "Konstruktor kopiujacy Playlist" << endl;
        }

        Playlist(Playlist&& other) noexcept : name{move(other.name)}, songs{move(other.songs)} {
            cout << "Konstruktor przenoszacy Playlist" << endl;
        }

        auto operator=(const Playlist& other) -> Playlist& {
            cout << "Kopiujacy operator przypisania Playlist" << endl;

            if (this != &other) {
                name = other.name;
                songs = other.songs;
            }

            return *this;
        }

        auto operator=(Playlist&& other) noexcept -> Playlist& {
            cout << "Przenoszacy operator przypisania" << endl;

            if (this != &other) {
                name = move(name);
                songs = move(songs);
            }

            return *this;
        }

        ~Playlist() {
            cout << "Destruktor Playlist: " << name << endl;
        }

        auto print() const -> void {
            cout << "--- Playlista " << name << " ---" << endl;
            if (songs.empty()) {
                cout << " (pusta)" << endl;
            } else {
                for (size_t i=0; i < songs.size(); ++i) {
                    cout << " " << i + 1 << ". " << songs[i] << endl;
                }
            }
        }
};

auto create_example_playlist() -> Playlist {
            Playlist p{"Example"};
            p.add_song("Sigma song");
            p.add_song("Sigma boy");
            
            return p;
}

auto main() -> int
{
    // Zadanie 1
    Book book1{"Clean Code", "Marthin C. Adams", 2000};
    Book book2{"Sigma", "Zyzz", 1967};

    book1.print();
    book2.print();

    // Zadanie 2
    Movie m{};
    m.print();

    m.set_duration(2);
    m.set_title("Spider-man");
    m.set_rating(10.0);
    m.print();

    // Zadanie 3
    cout << "===== Start zadania 3 =====" << endl;

    Task a{"Nauczyc sie klas"};
    // Task b{a};
    // Task b{"Przypisanie"};
    // b = a;
    a.mark_done();
    
    // Task b{move(a)};
    Task b{"Drugie zadanie"};
    b = move(a);
    a.print();
    // {
    //     Task b{"Obiekt z bloku"};
    //     b.print();
    // }
    b.print();

    cout << "===== Koniec programu =====" << endl;

    cout << "===== Zadanie Playlist =====" << endl;
    Playlist p{"Taco Hemingway"};
    p.add_song("Trojkat Warszawski");
    p.add_song("Bez stresu");
    p.add_song("W piatki leze w wannie");
    p.print();

    cout << "\n>>> 2. Test konstruktora kopiujacego" << endl;
    Playlist list_copy = p;
    list_copy.print();

    cout << "\n>>> 3. Test kopiujacego operatora przypisania" << endl;
    Playlist another_list{p};
    another_list.add_song("Golec uOrkiestra - Sciernisko");
    another_list.print();

    cout << "\n>>> 4. Test konstruktora przenoszacego" << endl;
    cout << "Kopia przed przeniesieniem" << endl;
    list_copy.print();

    Playlist list_moved = move(list_copy);
    cout << "Nowy obiekt po przeniesieniu: " << endl;
    list_moved.print();
    cout << "Stary obiekt (list_copy) po przeniesieniu: " << endl;
    list_copy.print();

    cout << "\n>>> 5. Test przenoszacego operatora przypisania" << endl;
    Playlist final_list("Pusta");
    final_list = move(p);
    final_list.print();

    cout << "\n>>> 6. Koniec programu" << endl;

    Playlist pl{"Sigma playlist"};
    Playlist example = create_example_playlist();
    example.print();


    pl = create_example_playlist();
    pl.print();
    
    return 0;
}
