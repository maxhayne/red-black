/*
Max Hayne
CSCI2270 Rhonda Hoenigman
Assignment 10
TA: Camilla Lambroco
*/

#include <iostream>
#include <string>
#include "MovieTree.h"
using namespace std;

int main(int argc, char* argv[]){
    bool isQuit = false;
    char* filename = argv[1];
    string title, dummy;
    int selection;
    MovieTree mt(filename);

    // Creating the menu
    while (isQuit == false){
        cout << "======Main Menu======" << endl;
        cout << "1. Find a movie" << endl;
        cout << "2. Rent a movie" << endl;
        cout << "3. Print the inventory" << endl;
        cout << "4. Delete a movie" << endl;
        cout << "5. Count the movies" << endl;
        cout << "6. Count the longest path" << endl;
        cout << "7. Quit" << endl;
        cin >> selection;

        // If statements based on user selection
        if (selection == 1){
            cout << "Enter title:" << endl;
            getline(cin, dummy);
            getline(cin, title);
            mt.findMovie(title);
        }

        else if (selection == 2){
            cout << "Enter title:" << endl;
            getline(cin, dummy);
            getline(cin, title);
            mt.rentMovie(title);
        }

        else if (selection == 3){
            mt.printMovieInventory();
        }

        else if (selection == 4){
            cout << "Enter title:" << endl;
            getline(cin, dummy);
            getline(cin, title);
            mt.deleteMovieNode(title);

        }

        else if (selection == 5){
            cout << "Tree contains: " << mt.countMovieNodes() << " movies." << endl;
        }

        else if (selection == 6){
            cout << "Longest Path: " << mt.countLongestPath() << endl;
        }

        else{
            isQuit = true;
        }

    }

    cout << "Goodbye!" << endl;
}
