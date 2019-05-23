/*
 * This program is a representation of George
 * Horton Conway's Game of Life.  It is a simulation of
 * the lifecycleof a bacteria colony.  Cells live
 * and die by a specific set of rules.
 *
 * Assignment 1 CS 106b
 * Stanford University
 * Glenn Langdon
*/

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
#include "lifegui.h"

using namespace std;

/*
 * Constant values
 */
const int cyclePause = 50;

/*
 *  Function prototypes
 */
void introMessage();
Grid<bool> drawOrigMatrix(ifstream &stream, int rows, int cols);
void boolsToChars(Grid<bool> &matrix);
int countCells(Grid<bool> matrix, int i, int j, bool wrap);
Grid<bool> makeNextGen(Grid<bool> matrix, bool wrap);
bool wrapPrompt();
void oneGeneration(Grid<bool> &matrix, bool wrap);
void fileList();


int main() {
    introMessage();
    fileList();

    ifstream stream;
    promptUserForFile(stream, "Grid input file name?");

    bool wrap = wrapPrompt();

    string rows, cols;
    getline(stream,rows);
    getline(stream, cols);
    int numRows = stringToInteger(rows);
    int numCols = stringToInteger(cols);

    Grid<bool> matrix = drawOrigMatrix(stream, numRows, numCols);
    boolsToChars(matrix);

    // Run simulation
    while (true) {
    string playSimPrompt = getLine("a)nimate, t)ick, q)uit");
    if(playSimPrompt == "t") {
        oneGeneration(matrix, wrap);
    }
    else if (playSimPrompt == "a") {
        int numFrames = getInteger("How many frames?");
        for (int i = 0; i < numFrames; i++) {
            oneGeneration(matrix, wrap);
            pause(cyclePause);
            clearConsole();
        }
    }
    else if (playSimPrompt == "q") {
        break;
    }
 }
    cout << endl;
    cout << "Have a nice Life!" << endl;
    return 0;
}

/*
 * Function displays introduction message and
 * instructions for use of the simulation.
 */
void introMessage() {
    cout << "Welcome to the CS 106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl;
    cout << endl;
}

/*
 * Function to take a reference to a stream,
 * determine the number of cols and rows, define
 * a matrix, and populate a default boolean matrix.
 */
Grid<bool> drawOrigMatrix(ifstream &stream, int rows, int cols) {
    string line;
    Grid<bool> matrix(rows, cols);
    for(int r = 0; r < rows; r++) {
        getline(stream, line);
        for(int c = 0; c < cols; c++) {
            if (line[c] == 'X'){
                matrix[r][c] = true;
            }
            else matrix[r][c] = false;
        }
    }
    stream.close();
    return matrix;
}

/*
 * A function that takes in a boolean matrix
 * and returns a character matrix to the console.
 */
void boolsToChars(Grid<bool> &matrix) {
    for(int i = 0; i < matrix.numRows(); i++) {
        for(int j = 0; j < matrix.numCols(); j++) {
            if (matrix[i][j]) {
                cout << 'X';
            }
            else cout << '-';
        }
    cout << endl;
    }
}

/*
 * Iterate through boolean matrix, intialize temporary
 * matrix to hold new values, adjust cell boolean values
 * according to rules and return temporary grid.
 */
Grid<bool> makeNextGen(Grid<bool> matrix, bool wrap) {
    int rows = matrix.numRows();
    int cols = matrix.numCols();
    Grid<bool> tempGrid(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int cellCount = countCells(matrix, i, j, wrap);
            if (cellCount <= 1) {
                tempGrid[i][j]  = false;
            }
            else if (cellCount == 2) {
                tempGrid[i][j] = matrix[i][j];
            }
            else if (cellCount == 3) {
                tempGrid[i][j] = true;
            }
            else if (cellCount == 4) {
                tempGrid[i][j] = false;
            }
        }
    }
    return matrix = tempGrid;
}

/*
 * This function takes in a matrix and
 * the number of rows and columns of the matrix.
 * It checks all neighboring cells
 * but not itself for a true value.  If true value
 * is found, the count is increased and returned.
 */
int countCells(Grid<bool> matrix, int row, int col,  bool wrap) {
    int numRows = matrix.numRows();
    int numCols = matrix.numCols();
    int count = 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (matrix.inBounds(row + i, j + col) &&
                    matrix[row + i][col + j] &&
                        (i != 0 || j != 0)) {
                count++;
            }
            else if (wrap && matrix[(row + i + numRows) % numRows][(col + j + numCols) % numCols] && (i != 0 || j != 0)) {
                count ++;
            }
        }
    }
    return count;
}

/*
 * This method returns a boolean if the simulation
 * is to wrap around the matrix from keyboard
 * input.
 */
bool wrapPrompt(){
    string prompt = "Should the simulation wrap around the grid?";
    if (getYesOrNo(prompt)) {
    return true;
    }
    else {
        return false;
    }
}

/*
 * This function takes a reference to a matrix,
 * combining two functions to make the main
 * program more readable.
 */
void oneGeneration(Grid<bool> &matrix, bool wrap){
    Grid<bool> nextGen = makeNextGen(matrix, wrap);
    boolsToChars(nextGen);
    matrix = nextGen;
}
/*
 * List of files to choose from.
 */
void fileList() {
    cout << "Choose a file from the list:" << endl;

    cout << "diehard.txt" << endl;
    cout << "diehard-copy.txt" << endl;
    cout << "dinner-table.txt" << endl;
    cout << "fish.txt" << endl;
    cout << "flower.txt" << endl;
    cout << "format.txt" << endl;
    cout << "glider-explosion" << endl;
    cout << "glider-gun.txt" << endl;
    cout << "glider.txt"<< endl;
    cout << "quilt-square.txt" << endl;
    cout << "rpent.txt" << endl;
    cout << "seeds.txt" << endl;
    cout << "simple.txt" << endl;
    cout << "snowflake" << endl;
    cout << "spiral.txt" << endl;
    cout << "stableplateau" << endl;
    cout << "tictactoe.txt"<< endl;
    cout << endl;
}
