/*
Max Hayne
CSCI2270 Rhonda Hoenigman
Assignment 10
TA: Camilla Lambroco
*/

#include "MovieTree.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

// Creating the binary search tree with the input of the filename
MovieTree::MovieTree(char* filename){
    MovieNode *nilNode = new MovieNode(-1, "", -1, -1); // nil node is created with throwaway values
    nilNode -> isRed = false; // nil node's color is set to black
    nil = nilNode;
    root = NULL; // root is set to NULL
    string line, temp, name;
    int rank, year, copies;
    ifstream inFile;
    stringstream ss;
    inFile.open(filename);
    if (!inFile.is_open()){
        cout << "File did not open!" << endl;
    }
    else{
        // Storing the strings in each line separated by a delimiter, then creating new nodes and adding them
        while (getline(inFile, line)){
            ss << line;
            for (int i = 0; i < 4; i++){
                getline(ss, temp, ',');
                if (i == 0){
                    rank = stoi(temp);
                }
                else if (i == 1){
                    name = temp;
                }
                else if (i == 2){
                    year = stoi(temp);
                }
                else{
                    copies = stoi(temp);
                }
            }
            addMovieNode(rank, name, year, copies);
            ss.clear();
        }
    }

    inFile.close();
}

// Deleting all movies from the tree when the program is finished
MovieTree::~MovieTree(){
    DeleteAll(root);
}

// Printing all movies
void MovieTree::printMovieInventory(){
    printMovieInventory(root);
}

// Counting the movies using a global variable for count
int MovieTree::countMovieNodes(){
    nodeCount = 0;
    countMovieNodes(root);
    return nodeCount;
}

// Deleting
void MovieTree::deleteMovieNode(string title){
    // tempSide tells which side the temp node is on
    int tempSide;
    MovieNode *temp = root;
    // Finding the temp node
    while (temp != nil){
        if (title > temp -> title){
            temp = temp -> rightChild;
            tempSide = 1;
        }
        else if (title < temp -> title){
            temp = temp -> leftChild;
            tempSide = 0;
        }
        else {
            break;
        }
    }

    // If the movie does not exist
    if (temp == nil){
        cout << "Movie not found." << endl;
        return;
    }

    bool tempColor = temp -> isRed;
    MovieNode *x = NULL;
    // Special case where movie being deleted is root
    if (temp == root){
        if (temp -> leftChild == nil){ // left child is nil
            if (temp -> rightChild != nil){ // rightchild is not nil
                temp -> rightChild -> parent = nil;
                root = temp -> rightChild;
                x = temp -> rightChild; // is set to the one child that the node being deleted has
            }
            else { // rightchild and leftchild are nil, therefore the node can just be deleted
                delete root;
                delete temp;
                return;
            }
        }

        else if (temp -> leftChild != nil){ // leftchild is not nil
            if (temp -> rightChild -> leftChild == nil){ // min is temp's rightchild
                temp -> rightChild -> parent = nil;
                temp -> rightChild -> leftChild = temp -> leftChild;
                temp -> leftChild -> parent = temp -> rightChild;
                root = temp -> rightChild;
                x = temp -> rightChild; // x is set to the node replacing temp, which is temp's right child
            }
        }
    }

    // If temp is not root ... special cases
    else if (temp -> leftChild == nil){ // leftchild is nil
        if (tempSide == 0){ // temp is a leftchild
            temp -> parent -> leftChild = temp -> rightChild;
            x = temp -> parent; // x is set to temp's parent, this assumes that the rightchild of temp is nil. If temp's rightchild is not nil
                                // the function will correct itself on the third if statement in this else if statement
        }
        if (tempSide == 1){ // temp is a rightchild
            temp -> parent -> rightChild = temp -> rightChild;
            x = temp -> parent; // x is set to temp's parent, this assumes that the rightchild of temp is nil. If temp's rightchild is not nil
                                // the function will correct itself in the next if statement
        }
        if (temp -> rightChild != nil){ // temp rightchild is not nil
            temp -> rightChild -> parent = temp -> parent;
            x = temp -> rightChild; // since the rightchild of temp is not nil, x is set to the right child of temp (the node that will be replacing temp)
        }
    }

    else if (temp -> leftChild != nil){ // leftchild is not nil
        if (temp -> rightChild -> leftChild == nil){ // min is temp's rightchild
            temp -> rightChild -> parent = temp -> parent;
            temp -> rightChild -> leftChild = temp -> leftChild;
            temp -> leftChild -> parent = temp -> rightChild;
            x = temp -> rightChild; // x is set to the node replacing temp (temp's rightchild)
            if (tempSide == 0){ // temp is a leftchild
                temp -> parent -> leftChild = temp -> rightChild;
            }
            if (tempSide == 1){ // temp is rightchild
                temp -> parent -> rightChild = temp -> rightChild;
            }
        }
    }
    if (x == NULL){ // non of the special cases have been met, so enter into this if-statement
        // Non special case of finding the minimum
        MovieNode *minP = temp;
        MovieNode *min = temp -> rightChild;

        while (min != nil){
            minP = min;
            min = min -> leftChild;
        }

        min = minP;
        minP = minP -> parent;
        tempColor = min -> isRed; // tempColor is set to the minimum value's color

        // Replacing temp information with min information
        temp -> quantity = min -> quantity;
        temp -> year = min -> year;
        temp -> title = min -> title;
        temp -> ranking = min -> ranking;
        temp -> isRed = min -> isRed;

        minP -> leftChild = min -> rightChild;
        if (min -> rightChild != nil){ // min's rightchild's parent is min's parent
            min -> rightChild -> parent = minP;
            x = min -> rightChild; // x is set to min's rightchild (if and only if min's rightchild is not nil)
        }
        else{
            x = minP; // min is set to min's parent, if min does not have a rightchild
        }

        if (tempColor == false){ // if tempColor is black, that means that the deletion could have caused an imbalance in the tree, so
                                 // a rebalance must be called on x
            rbDeleteFixup(x);
        }
        return;
    }

    if (tempColor == false){ // if tempColor is black, it is possible that there is an imbalance in the tree, so enter into the if-statement
        if (x != NULL){ // if tempColor is black and x does not equal NULL (it has been changed to something else throughout the function),
                        // deleteFixup is called on x
            rbDeleteFixup(x);
        }
    }

    delete temp; // temp is deleted
}

// Adding a movie to the tree
void MovieTree::addMovieNode(int ranking, string title, int releaseYear, int quantity){
    MovieNode *newNode = new MovieNode(ranking, title, releaseYear, quantity);
    newNode -> leftChild = nil;
    newNode -> rightChild = nil;
    if (root == NULL){
        newNode -> parent = nil;
        newNode -> isRed = false;
        root = newNode;
    }
    else{
        MovieNode *current = root;
        MovieNode *cParent = nil;
        while (current != nil){
            if (newNode -> title > current -> title){
                cParent = current;
                current = current -> rightChild;
            }
            else{
                cParent = current;
                current = current -> leftChild;
            }
        }
        newNode -> parent = cParent;
        if (title > cParent -> title){
            cParent -> rightChild = newNode;
        }
        else{
            cParent -> leftChild = newNode;
        }
    }

    rbAddFixup(newNode); // after movie is added, addFixup is called on the added node to rebalance the tree
}

// Finding a movie
void MovieTree::findMovie(string title){
    MovieNode *current = root;
    while (current != nil){
        if (title > current -> title){
            current = current -> rightChild;
        }
        else if (title < current -> title){
            current = current -> leftChild;
        }
        else{
            break;
        }
    }
    if (current == nil){
        cout << "Movie not found." << endl;
    }
    else{
        cout << "Movie Info:" << endl;
        cout << "===========" << endl;
        cout << "Ranking:" << current -> ranking << endl;
        cout << "Title:" << current -> title << endl;
        cout << "Year:" << current -> year << endl;
        cout << "Quantity:" << current -> quantity << endl;
    }
}

// Renting a movie
void MovieTree::rentMovie(string title){
    MovieNode *current = root;
    while (current != nil){
        if (title > current -> title){
            current = current -> rightChild;
        }
        else if (title < current -> title){
            current = current -> leftChild;
        }
        else{
            break;
        }
    }
    if (current == nil){
        cout << "Movie not found." << endl;
        return;
    }
    else{
        current -> quantity--;
        cout << "Movie has been rented." << endl;
        cout << "Movie Info:" << endl;
        cout << "===========" << endl;
        cout << "Ranking:" << current -> ranking << endl;
        cout << "Title:" << current -> title << endl;
        cout << "Year:" << current -> year << endl;
        cout << "Quantity:" << current -> quantity << endl;
    }
    if (current -> quantity == 0){
        deleteMovieNode(title);
    }

}

// Deleting all using a post-order traversal
void MovieTree::DeleteAll(MovieNode *node){ //use this for the post-order traversal deletion of the tree
    MovieNode *temp = node;
    if (temp -> leftChild != nil){
        DeleteAll(temp -> leftChild);
    }
    if (temp -> rightChild != nil){
        DeleteAll(temp -> rightChild);
    }
    cout << "Deleting: " << temp -> title << endl;
    delete temp;
}

// Printing using In-order traversal
void MovieTree::printMovieInventory(MovieNode *node){
    MovieNode *temp = node;
    if (temp -> leftChild != nil){
        printMovieInventory(temp -> leftChild);
    }
    cout << "Movie: " << temp -> title << " " << temp -> quantity << endl;
    if (temp -> rightChild != nil){
        printMovieInventory(temp -> rightChild);
    }
}

// Counting movies using a global variable for count
void MovieTree::countMovieNodes(MovieNode *node){
    MovieNode *temp = node;
    if (temp -> leftChild != nil){
        countMovieNodes(temp -> leftChild);
    }
    nodeCount++;
    if (temp -> rightChild != nil){
        countMovieNodes(temp -> rightChild);
    }
}

// leftRotate function
void MovieTree::leftRotate(MovieNode *x){
    MovieNode *y = x -> rightChild; // y is set to x's rightchild
    x -> rightChild = y -> leftChild; // x's rightchild is set to y's leftchild
    if (y -> leftChild != nil){ // if y's leftchild is not empty, y's leftchild's parent is set to x
        y -> leftChild -> parent = x;
    }
    y -> parent = x -> parent; // y's parent is set to x's parent
    if (x -> parent == nil){ // if x is the root, y is set to the root
        root = y;
    }
    else{ // if x is not the root, y is set to the left or rightchild of x's parent, depending on which child x is
        if (x == x -> parent -> leftChild){
            x -> parent -> leftChild = y;
        }
        else{
            x -> parent -> rightChild = y;
        }
    }
    y -> leftChild = x; // y's leftchild is set to x
    x -> parent = y; // x's parent is set to y
}

// rightRotate function
void MovieTree::rightRotate(MovieNode *y){
    MovieNode *x = y -> leftChild; // x is equal to y's leftchild
    y -> leftChild = x -> rightChild; // y's leftchild rerouted to x's rightchild
    if (x -> rightChild != nil){ // if x's rightchild is not empty, x's rightchild's parent is set to y
        x -> rightChild -> parent = y;
    }
    x -> parent = y -> parent; // x's parent is equal to y's parent
    if (y -> parent == nil){ // if y's parent is nil, then y is the root, and therefore x is set to the new root
        root = x;
    }
    else if (y == y -> parent -> leftChild){ // if y is the leftchild of its parent, y's parent's leftchild is set to x
        y -> parent -> leftChild = x;
    }
    else{ // if y is the rightchild of its parent, y's parent's rightchild is set to x
        y -> parent -> rightChild = x;
    }
    x -> rightChild = y; // x's rightchild is set to y
    y -> parent = x; // y's parent is set to x
}

// rbValid function, didn't really touch this, only used it for checking the tree
int MovieTree::rbValid(MovieNode *node){
    int lh = 0;
    int rh = 0;

    // If we are at a nil node just return 1
    if (node == nil)
        return 1;

    else
    {
        // First check for consecutive red links.
        if (node->isRed)
        {
            if(node->leftChild->isRed || node->rightChild->isRed)
            {
                cout << "This tree contains a red violation" << endl;
                return 0;
            }
        }

        // Check for valid binary search tree.
        if ((node->leftChild != nil && node->leftChild->title.compare(node->title) > 0) || (node->rightChild != nil && node->rightChild->title.compare(node->title) < 0))
        {
            cout << "This tree contains a binary tree violation" << endl;
            return 0;
        }

        // Deteremine the height of left and right children.
        lh = rbValid(node->leftChild);
        rh = rbValid(node->rightChild);

        // black height mismatch
        if (lh != 0 && rh != 0 && lh != rh)
        {
            cout << "This tree contains a black height violation" << endl;
            return 0;
        }

        // If neither height is zero, incrament if it if black.
        if (lh != 0 && rh != 0)
        {
                if (node->isRed)
                    return lh;
                else
                    return lh+1;
        }

        else
            return 0;
    }
}

// Fixup function for deleting nodes from the tree. Pseudo code from Rhonda's textbook was used to help with the case where
// x is the leftchild (First half of while)
void MovieTree::rbDeleteFixup(MovieNode *node){
    MovieNode *y;
    MovieNode *x = node;
    while ((x != root) && (x -> isRed == false)){
        if (x == x -> parent -> leftChild){ // x is leftchild
            y = x -> parent -> rightChild; // y is the rightchild of x's parent (sibling)
            if (y -> isRed == true){ // If y is red
                y -> isRed = false;
                x -> parent -> isRed = true;
                leftRotate(x -> parent);
                y = x -> parent -> rightChild;
            }
            if ((y -> leftChild -> isRed == false) && (y -> rightChild -> isRed == false)){ // if both of y's children are black nodes
                y -> isRed = true;
                x = x -> parent;
            }
            else if ((y -> leftChild -> isRed == true) && (y -> rightChild -> isRed == false)){ // if y's leftchild is red and rightchild is black
                y -> leftChild -> isRed = false;
                y -> isRed = true;
                rightRotate(y);
                y = x -> parent -> rightChild;
            }
            else{ // if y's children are both red or y's rightchild is red and y's leftchild is black
                y -> isRed = x -> parent -> isRed;
                x -> parent -> isRed = false;
                y -> rightChild -> isRed = false;
                leftRotate(x -> parent);
                x = root;
            }
        }
        else{
            // x is a right child
            // exchange left and right
            y = x -> parent -> leftChild; // y is x's sibling
            if (y -> isRed == true){ // if y is red
                y -> isRed = false;
                x -> parent -> isRed = true;
                rightRotate(x -> parent);
                y = x -> parent -> leftChild;
            }
            if ((y -> rightChild -> isRed == false) && (y -> leftChild -> isRed == false)){ // if both of y's children are black
                y -> isRed = true;
                x = x -> parent;
            }
            else if ((y -> rightChild -> isRed == true) && (y -> leftChild -> isRed == false)){ // if y's rightchild is red and leftchild is black
                y -> rightChild -> isRed = false;
                y -> isRed = true;
                leftRotate(y);
                y = x -> parent -> leftChild;
            }
            else{ // if both of y's children are red or y's leftchild is red and y's rightchild is black
                y -> isRed = x -> parent -> isRed;
                x -> parent -> isRed = false;
                y -> leftChild -> isRed = false;
                rightRotate(x -> parent);
                x = root;
            }
        }
    }

    x -> isRed = false; // x's color is changed to black, gets to here if x is the root or x is already black
}

// Fixup function for adding nodes to the tree. Pseudo code from Rhonda's textbook and Recitation 10 helped me create this function
void MovieTree::rbAddFixup(MovieNode *node){
    MovieNode *x = node;
    MovieNode *uncle;
    while (x != root && x -> parent -> isRed == true){
        if (x -> parent == x -> parent -> parent -> leftChild){ // x -> parent is the leftchild
            uncle = x -> parent -> parent -> rightChild; // uncle is the opposite child of the parent
            if (uncle -> isRed == true){ // Case where uncle is red
                //RBCase1Left(x, uncle)
                x -> parent -> isRed = false;
                uncle -> isRed = false;
                x -> parent -> parent -> isRed = true;
                x = x -> parent -> parent;
            }
            else{ // Case where uncle is black
                if (x == x -> parent -> rightChild){ // If x is a rightchild
                    x = x -> parent;
                    leftRotate(x);
                }
                //RBCase3Left(x)
                x -> parent -> isRed = false;
                x -> parent -> parent -> isRed = true;
                rightRotate(x -> parent -> parent);
            }
        }
        else{ // x -> parent is the leftchild
            uncle = x -> parent -> parent -> leftChild;
            if (uncle -> isRed == true){ // If uncle is red
                //RBCase1Left(x, uncle)
                x -> parent -> isRed = false;
                uncle -> isRed = false;
                x -> parent -> parent -> isRed = true;
                x = x -> parent -> parent;
            }
            else{ // If uncle is black
                if (x == x -> parent -> leftChild){ // x is a leftchild
                    x = x -> parent;
                    rightRotate(x);
                }
                //RBCase3Right(x)
                x -> parent -> isRed = false;
                x -> parent -> parent -> isRed = true;
                leftRotate(x -> parent -> parent);
            }
        }
    }

    root -> isRed = false; // Set root to black
}

// Public function that calls countLongestPath on the root
int MovieTree::countLongestPath(){
    return countLongestPath(root);
}

// Recursive function that steps through each node in the tree, and returns the maximum number of nodes
// stepped through in the tree.
int MovieTree::countLongestPath(MovieNode *node){
    if (node == nil){
        return 0;
    }

    return 1 + max(countLongestPath(node -> leftChild), countLongestPath(node -> rightChild));
}

