/*
Max Hayne
CSCI2270 Rhonda Hoenigman
Assignment 10
TA: Camilla Lambroco
*/

#ifndef MOVIETREE_H
#define MOVIETREE_H

#include <string>
using namespace std;

struct MovieNode{
    int ranking;
    string title;
    int year;
    int quantity;
    MovieNode *parent;
    MovieNode *leftChild;
    MovieNode *rightChild;
    bool isRed;

    MovieNode(){};

    MovieNode(int in_ranking, string in_title, int in_year, int in_quantity)
    {
        ranking = in_ranking;
        title = in_title;
        year = in_year;
        quantity = in_quantity;
        parent = NULL;
        leftChild = NULL;
        rightChild = NULL;
        isRed = true;
    }

};

class MovieTree
{
    public:
        MovieTree(char* filename);
        ~MovieTree();
        void printMovieInventory();
        int countMovieNodes();
        void deleteMovieNode(string title);
        void addMovieNode(int ranking, string title, int releaseYear, int quantity);
        void findMovie(string title);
        void rentMovie(string title);
        bool isValid();
        int countLongestPath();

    protected:

    private:
        void printMovieInventory(MovieNode * node);
        void countMovieNodes(MovieNode *node);
        void DeleteAll(MovieNode *node);
        void rbAddFixup(MovieNode *node); // called after insert to fix tree
        void leftRotate(MovieNode *x); //rotate the tree left with x as the root of the rotation
        void rightRotate(MovieNode *x); //rotate the tree right with x as the root of the rotation
        void rbDeleteFixup(MovieNode *node); //called after delete to fix the tree
        int rbValid(MovieNode *node); //check if the tree is valid, with node as the root of the tree
        int countLongestPath(MovieNode *node); //longest path from node to a leaf node in the tree
        MovieNode* root;
        MovieNode* nil;
        int nodeCount;
};

#endif // MOVIETREE_H

