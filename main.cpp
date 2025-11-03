// COMSC-210 | Lab 28 | Andrei Buchatskiy
// IDE used : Visual Studio Code


#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <list>
// need to include numeric for the accumulate
#include <numeric>
// include rnaodm for the shuffle
#include <random>
#include "Goat.h"
using namespace std;

const int SZ_NAMES = 200, SZ_COLORS = 25;

int select_goat(list<Goat> trip);
void delete_goat(list<Goat> &trip);
void add_goat(list<Goat> &trip, string [], string []);
void display_trip(list<Goat> trip);
int main_menu();

// function prototype for goat age check
// age_checlk(): checks if any goat is at the given age
// arguments: i shoudl use a const here to be honest since i dont want to modify the list
// returns: nothing
void age_check(const list<Goat> &trip);

// avg_age(): calculates average age of the trip using accumulate
void avg_age(const list<Goat> &trip);

// reverse_trip(): reverses the order of the list of goats
void reverse_trip(list<Goat> &trip);

// replace_goat(): replaces the entire goat
void replace_goat(list<Goat> &trip);

// find_goat(): finds the goat by the given name
void find_goat(const list<Goat> &trip);

// clear_trip(): clears the list of goats
void clear_trip(list<Goat> &trip);

// paint_goats(): changes goats colors to given color
void paint_goats(list<Goat> &trip);

// shuffle_trip(): changes order of the goats
void shuffle_trip(list<Goat> &trip);

int main() {
    srand(time(0));
    bool again;

    // read & populate arrays for names and colors
    ifstream fin("names.txt");
    string names[SZ_NAMES];
    int i = 0;
    while (fin >> names[i++]);
    fin.close();
    ifstream fin1("colors.txt");
    string colors[SZ_COLORS];
    i = 0;
    while (fin1 >> colors[i++]);
    fin1.close();

    // create & populate a trip of Goats using std::list of random size 8-15
    int tripSize = rand() % 8 + 8;
    list<Goat> trip;
    int age;
    string name, color;
    for (int i = 0; i < tripSize; i++) {
        age = rand() % MAX_AGE;  // defined in Goat.h
        name = names[rand() % SZ_NAMES];
        color = colors[rand() % SZ_COLORS];
        Goat tmp(name, age, color);
        trip.push_back(tmp);
    }
    
    // Goat Manager 3001 Engine
    int sel = main_menu();
    while (sel != 12) {
        switch (sel) {
            case 1:
                cout << "Adding a goat.\n";
                add_goat(trip, names, colors);
                break;
            case 2:    
                cout << "Removing a goat.\n";
                delete_goat(trip);
                break;
            case 3:    
                cout << "Displaying goat data.\n";
                display_trip(trip);
                break;
            // oops i accidentally wrote 6 here should be 4
            case 4:
                cout << "Checking for goats who are given age.\n";
                age_check(trip);
                // i was missing break here
                break;
            case 5:
                cout << "Showing average age.\n";
                avg_age(trip);
                break;
            case 6:
                cout << "Reversing the order of the trip.\n";
                reverse_trip(trip);
                break;
            case 7:
                cout << "Replacing goat.\n";
                replace_goat(trip);
                break;
            case 8:
                cout << "Searching for goat.\n";
                find_goat(trip);
                break;
            case 9:
                cout << "Clearing the trip.\n";
                clear_trip(trip);
                break;
            case 10:
                cout << "Painting goats.\n";
                paint_goats(trip);
                break;
            case 11:
                cout << "Shuffling the goats.\n";
                shuffle_trip(trip);
                break;
            default:
                cout << "Invalid selection.\n";
                break;
        }
        sel = main_menu();
    }
    

    return 0;
}

int main_menu() {
    // to start I will add a new option to check goat ages, so imma use any_of for my milestone 1 algorithm
    // for milestone 2 imma use accumulate to find the avg age
    // milestone 3 will be just reversing the trip order
    // milestone 4 will use replace to replace the ages of goats with given age (gonna change this to replcing the whole goat)
    // milestone 5 will be find, so i will find the goat by its given name and otehr data, since i alr made the operator== in goat.h may as well use it again
    // milestone 6 will be clearing the trip
    //milestone 7 will be using fill to change all goats color
    // last mielstone will just be shuffling the trip
    cout << "*** GOAT MANAGER 3001 ***\n";
    cout << "[1] Add a goat\n";
    cout << "[2] Delete a goat\n";
    cout << "[3] List goats\n";
    cout << "[4] Check if any goat is given age" << endl;
    cout << "[5] Display average age" << endl;
    cout << "[6] Reverse trip order" << endl;
    cout << "[7] Replace goat" << endl;
    cout << "[8] Find goat" << endl;
    cout << "[9] Clear the trip" << endl;
    cout << "[10] Paint goats" << endl;
    cout << "[11] Shuffle the trip" << endl;
    cout << "[12] Quit\n";
    cout << "Choice --> ";
    int choice;
    cin >> choice;
    while (choice < 1 || choice > 12) {
        cout << "Invalid, again --> ";
        cin >> choice;
    }
    return choice;
}

void delete_goat(list<Goat> &trip) {
    cout << "DELETE A GOAT\n";
    int index = select_goat(trip);
    auto it = trip.begin();
    advance(it, index-1);
    trip.erase(it);
    cout << "Goat deleted. New trip size: " << trip.size() << endl;
}

void add_goat(list<Goat> &trip, string nms[], string cls[]) {
    cout << "ADD A GOAT\n";
    int age = rand() % MAX_AGE;
    string nm = nms[rand() % SZ_NAMES];
    string cl = cls[rand() % SZ_COLORS];
    Goat tmp(nm, age, cl);
    trip.push_back(tmp);
    cout << "Goat added. New trip size: " << trip.size() << endl;
}

void display_trip(list<Goat> trp) {
    int i = 1;
    for (auto gt: trp)
        cout << "\t" 
             << "[" << i++ << "] "
             << gt.get_name() 
             << " (" << gt.get_age() 
             << ", " << gt.get_color() << ")\n";
}

int select_goat(list<Goat> trp) {
    int input;
    cout << "Make a selection:\n";
    display_trip(trp);
    cout << "Choice --> ";
    cin >> input;

    // was getting an error with the or here had to change it to ||
    while (input < 1 || input > trp.size()) {
        cout << "Invalid choice, again --> ";
        cin >> input;
    }
    return input;
}

void age_check(const list<Goat> &trip){
    // first check if the list is empty
    if(trip.empty()){
        cout << "Trip is empty" << endl;
        return;
    }
    
    // prompt and collect age
    int age;
    cout << "Enter age: ";
    cin >> age;

    // i was having trouble with the variable to put instead of 'int score' in the lesson notes, but i think i got it
    // i needed to make the [] into [&] in order for the age to be checked with the given age
    bool ageCheck = any_of(trip.begin(), trip.end(), [&](const Goat &g) { return g.get_age() == age; });

    if(ageCheck){
        cout << "Yes, there is a goat who is " << age << " years old" << endl;
    }
    else {
        cout << "No goat is " << age << " years old" << endl;
    }
}

void avg_age(const list<Goat> &trip){
    // first check if the list is empty
    if(trip.empty()){
        cout << "Trip is empty" << endl;
        return;
    }

    // use accumulate to get the sum of the goats ages
    // i need to pull get age for each goat, similar to how it was done in any_of
    int sum = accumulate(trip.begin(), trip.end(), 0,[](int a, const Goat &g){return a + g.get_age();});

    // now calculate the average of these ages, divide sum by the trip size
    // need to add (double) in front of sum to avoid integer division
    double avg = (double) sum / trip.size();

    // imma set precision to three here
    cout << fixed << setprecision(3) << "Average age: " << avg << " years" << endl;
}

void reverse_trip(list<Goat> &trip){
    // check if list has atleast two goats
    if(trip.size() < 2){
        cout << "Add more goats before trying to reverse" << endl;
        return;
    }

    //reverse the trip then display the new order
    reverse(trip.begin(),trip.end());
    cout << "Trip reversed:" << endl;
    display_trip(trip);
}

void replace_goat(list<Goat> &trip){
     // first check if the list is empty
    if(trip.empty()){
        cout << "Trip is empty" << endl;
        return;
    }

    // vars for old and new ages, and old namesold colors and new names nad colors
    // oopsi copy pasted the variable and made them all ints
    int oldAge;
    int newAge;
    string oldColor;
    string newColor;
    string oldName;
    string newName;

    // prompt for ages
    cout << "Enter name of goat to replace: ";
    cin >> oldName;
    cout << "Enter age of goat to replace: ";
    cin >> oldAge;
    cout << "Enter color of goat to replace: ";
    cin >> oldColor;


    cout << "Enter new age: ";
    cin >> newName;
    cout << "Enter new age: ";
    cin >> newAge;
    cout << "Enter new age: ";
    cin >> newColor;

    // use replace, imma need to set something up in Goat.h in order to compare elements
    // instead of 0 and -1 i need to put Goats with just ages in them
    // imma need to change this since replace replaces the entire element with just the age, so now imma need to change this to be more specific, you haveto give everything about the goat and fully replace it
    replace(trip.begin(), trip.end(), Goat(oldName, oldAge, oldColor), Goat(newName, newAge, newColor));

    cout << "Goat replaced, new trip:" << endl;
    display_trip(trip);
}

void find_goat(const list<Goat> &trip){
    // first check if the list is empty
    if(trip.empty()){
        cout << "Trip is empty" << endl;
        return;
    }

    // variables to search for
    string name;
    string color;
    int age;

    // prompt user
    cout << "Enter goat's name: ";
    cin >> name;
    cout << "Enter goat's age: ";
    cin >> age;
    cout << "Enter goat's color: ";
    cin >> color;

    // make a goat to use for searching
    Goat x(name,age,color);
    auto it = find(trip.begin(),trip.end(), x);
    if(it != trip.end()){
        cout << "Goat found" << endl;
    } 
    else {
        cout << "Goat not found" << endl;
    }
}

void clear_trip(list<Goat> &trip){
    // first check if the list is empty
    if(trip.empty()){
        cout << "Trip is empty already" << endl;
        return;
    }

    // use .clear()
    trip.clear();

    cout << "Goats removed from the trip. New trip size: " << trip.size() << endl;
}

void paint_goats(list<Goat> &trip){
    // first check if the list is empty
    if(trip.empty()){
        cout << "Trip is empty" << endl;
        return;
    }

    // prompt for new color to paint the goats, i want to demonstrate fill usage so in reality im gonna make all the goats lose their name and age, and set their color to the new color
    string newColor;
    cout << "Enter color to paint goats: ";
    cin >> newColor;

    // not ideal for painting the goats cus i am kind of erasing them and changing their color but i just want to demonstrate fill usage so its fine
    Goat gNew("", 0, newColor);

    fill(trip.begin(),trip.end(),gNew);
    cout << "All goats are now " << newColor << endl;
}

void shuffle_trip(list<Goat> &trip){
    // check if list has atleast two goats
    if(trip.size() < 2){
        cout << "Add more goats before trying to shuffle" << endl;
        return;
    }

    // shuffle the goats
    // getting errors with the shuffle looks like it doesnt work if it is a list, gonna make it a vecotr then put it back into the list post shuffle
    vector<Goat> tempTrip(trip.begin(),trip.end());

    shuffle(tempTrip.begin(),tempTrip.end(), default_random_engine());

    //clear the old list and put the shuffled one bnack into it
    trip.clear();
    // use a for loop to push_back the values into the original list
    for(const Goat &g : tempTrip){
        trip.push_back(g);
    }

    cout << "Goats were shuffled:" << endl;
    display_trip(trip);
}
