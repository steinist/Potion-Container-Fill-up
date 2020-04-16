/*
 * main.cpp
 *
 *  Created on: Sep 19, 2019
 *      Author: kevinw
 */

#include <iostream>
#include <cstring>
using namespace std;

const int ARRAY_SIZE = 12;
const char POTION = 'o';
const char RIM = 'T';
const char WALL = '*';
const char EMPTY = ' ';

enum TASK {CLEANUP, WARMUP_FILL, SIMPLE_FILL, ADVANCED_FILL};
void print_array(const char array[][ARRAY_SIZE+1]) {
    cout << "  ";
    for (int j = 0; j < ARRAY_SIZE; j++)
        cout << j % 10;
    cout << endl;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        cout << i % 10 << " ";
        cout << array[i] << endl;
    }
}

//HELPER FUNCTION
int find_level(char array[][ARRAY_SIZE+1], int col = 0, int level = 0){
    if (level >= ARRAY_SIZE)
        return -1;

    if (col >= ARRAY_SIZE){
        level++;
        col = 0;
    }

    if (array[level][col] == RIM)
        return level;

    else
        return find_level(array, col + 1, level);
}

bool is_spill(char array[][ARRAY_SIZE+1], int level = 0, int col = 0){
    if (level >= ARRAY_SIZE)
        return false;

    if (array[level][0] == POTION || array[level][ARRAY_SIZE-1] == POTION)
        return true;

    if (array[ARRAY_SIZE-1][col] == POTION)
        return true;

    return is_spill(array, level + 1, col + 1);
}

bool is_spill_add(char array[][ARRAY_SIZE + 1], int row = 0, int col = 0){
    if (col >= ARRAY_SIZE)
        return false;

    if (row >= ARRAY_SIZE){
        col++;
        row = 0;
    }
    if (array[row][col] == RIM && (array[row - 1][col] == POTION || array[row - 1][col - 1] == POTION || array[row - 1][col + 1] == POTION)){
        return true;
    }
    else
        return is_spill_add(array, row + 1, col);
}

int num_potion(char array[][ARRAY_SIZE + 1], int row = 0, int col = 0, int num = 0){
    if (col >= ARRAY_SIZE)
        return 0;

    if (row >= ARRAY_SIZE){
        col++;
        row = 0;
    }

    if (array[row][col] == POTION){
        return 1 + num_potion(array, row + 1, col, num + 1);
    }

    else
        return num_potion(array, row + 1, col, num);
}

bool above_broken(char array[][ARRAY_SIZE+1], int level, bool from_left = true, int col = 0){
    if (array[level][col] == RIM) {
        return false;
    }

    if (array[level][col] == WALL){
        if (from_left)
            return above_broken(array, level, from_left, col + 1);
        else
            return above_broken(array, level, from_left, col - 1);
    }
    if (array[level + 1][col] == POTION || array[level][col] == POTION) {
        return true;
    }

    if (array[level + 1][col] == WALL || array[level + 1][col] == EMPTY) {
        if (from_left)
            return above_broken(array, level, from_left, col + 1);
        else
            return above_broken(array, level, from_left, col - 1);
    }


}

int cleanup(char array[ARRAY_SIZE][ARRAY_SIZE+1], int row = 0, int col = 0) {
// Don't forget to return!

    if (col >= ARRAY_SIZE)
        return 0;

    if (row >= ARRAY_SIZE){
        col++;
        row = 0;
    }

    if (array[row][col] == POTION){
        array[row][col] = EMPTY;
        return 1 + cleanup(array, row + 1, col);
    }

    else
        return cleanup(array, row + 1, col);
}


void warmup_fill(char array[][ARRAY_SIZE+1], int row, int col, int level) {

    if (array[row + level][col] != EMPTY)
        return;
    else
        array[row + level][col] = POTION;

    if (row + level + 1 < ARRAY_SIZE)
        warmup_fill(array, row + 1, col, level);
    if (col + 1 < ARRAY_SIZE)
        warmup_fill(array, row, col + 1, level);
    if (col - 1 > 0)
        warmup_fill(array, row, col - 1, level);
    if (row - 1 >= 0)
        warmup_fill(array, row - 1, col, level);
}

void simple_fill(char array[][ARRAY_SIZE+1], int row, int col) {
    int rim_level = find_level(array, col, row);

    warmup_fill(array, 0, col, rim_level);

    cout << "level: " << rim_level << endl;
    cout << "col: " << col << endl;

//    print_array(array);
    if (is_spill(array) || is_spill_add(array)) {
        cleanup(array);
        simple_fill(array, rim_level + 1,col);
    }

    else
        return;
}


//int advanced_fill(char array[][ARRAY_SIZE+1], int row, int col) {
//// Don't forget to return!
//    int rim_level = find_level(array, col);
//    cout << "level: " << rim_level << endl;
//
//    warmup_fill2(array, row, col, rim_level);
//
//    bool spill = is_spill(array);
//
//    if (spill){
//        cout << "SPILL !!!" << endl;
//        return cleanup(array);
//    }
//}



int advanced_fill(char array[][ARRAY_SIZE+1], int row, int col) {
// Don't forget to return!
    int rim_level = find_level(array);

    if (array[row + rim_level][col] == EMPTY){
        if (row == 0 ||
        array[row + rim_level - 1][col] == POTION ||
        array[row + rim_level][col - 1] == POTION ||
        array[row + rim_level + 1][col] == POTION ||
        array[row + rim_level][col + 1] == POTION){
            array[row + rim_level][col] = POTION;
        }
    }

    else {
        return 1;}
//    print_array(array);
    if (is_spill(array, rim_level) || above_broken(array, rim_level) || above_broken(array, rim_level, false, ARRAY_SIZE-1)) {
        cout << "BROKEN";
        cleanup(array);
        return 0;
    }


    int potion = num_potion(array);
    if (potion == 0)
        return 0;

    //Down
    if (row + rim_level + 1 < ARRAY_SIZE) {
        advanced_fill(array, row + 1, col);
    }
    //Right
    if (col + 1 < ARRAY_SIZE) {
        advanced_fill(array, row, col + 1);
    }
    //Left
    if (col - 1 > 0) {
        advanced_fill(array, row, col - 1);
    }
    //Up
    if (row - 1 > 0) {
        advanced_fill(array, row - 1, col);
    }
}



void fill(int algorithm, char array[][ARRAY_SIZE+1], int row, int col) {
    cout << "Before Water is filled" << endl;
    print_array(array);
    int level;
    switch (algorithm) {
        case CLEANUP:
            cout << "cln:" << cleanup(array) <<endl;
            break;
        case WARMUP_FILL:
            cout << "Please help me mark the level:" << endl;
            cin >> level;
            warmup_fill(array, row, col, level);
            cout << "Warmup Fill" << endl;
            break;
        case SIMPLE_FILL:
            simple_fill(array, row, col);
            cout << "Simple Fill" << endl;
            break;
        case ADVANCED_FILL:
            advanced_fill(array,row,col);
            cout << "Advanced Fill" << endl;
            break;
    }
    print_array(array);
}




int main() {
    char test_tube[ARRAY_SIZE][ARRAY_SIZE+1] = {
            "            ",
            "            ",
            "            ",
            "            ",
            " ***T T***  ",
            " *       *  ",
            " *       *  ",
            " *       *  ",
            " *       *  ",
            " *********  ",
            "            ",
            "            "
    };


    char two_tubes[ARRAY_SIZE][ARRAY_SIZE+1] = {
            "            ",
            "     T T    ",
            "     ***    ",
            "            ",
            "     T T    ",
            "     * *    ",
            "     * *    ",
            "     * *    ",
            "     * *    ",
            "     ***    ",
            "            ",
            "            "
    };


    char conical_flask[ARRAY_SIZE][ARRAY_SIZE+1] = {
            "            ",
            "            ",
            "            ",
            "            ",
            "     T T    ",
            "     * *    ",
            "     * *    ",
            "    *   *   ",
            "   *     *  ",
            "  ********* ",
            "            ",
            "            "
    };


    char broken_tube[ARRAY_SIZE][ARRAY_SIZE+1] = {
            "            ",
            "            ",
            "            ",
            "            ",
            "     T T    ",
            "     * *    ",
            "     * *    ",
            "     * *    ",
            "     * *    ",
            "            ",
            "            ",
            "            "
    };

    char broken_flask[ARRAY_SIZE][ARRAY_SIZE+1] = {
            "            ",
            "            ",
            "            ",
            "            ",
            "     T T    ",
            "     * *    ",
            "     * *    ",
            "    *   *   ",
            "         *  ",
            "  ********* ",
            "            ",
            "            "
    };
    char beaker[ARRAY_SIZE][ARRAY_SIZE+1] = {
            "            ",
            "            ",
            "            ",
            "            ",
            "  T    T    ",
            "  *    *    ",
            "  *    *    ",
            "  *    *    ",
            "  *    *    ",
            "  ******    ",
            "            ",
            "            "
    };



    char flask[ARRAY_SIZE][ARRAY_SIZE+1] = {
            "            ",
            "    T T     ",
            "    * *     ",
            "    * *     ",
            "  **   **   ",
            " *       *  ",
            " *       *  ",
            " *       *  ",
            "  *     *   ",
            "   *****    ",
            "            ",
            "            "
    };


    char up_and_down[ARRAY_SIZE][ARRAY_SIZE+1] = {
            "            ",
            "            ",
            "            ",
            "            ",
            " T T        ",
            " * * *******",
            " * * *   * *",
            " * *** * * *",
            " *     *   *",
            " ********* *",
            " *         *",
            " ***********"
    };

    char broken_u_flask[ARRAY_SIZE][ARRAY_SIZE+1] = {
            "            ",
            "            ",
            "            ",
            "            ",
            "    T T     ",
            "    * * ****",
            "    * ***  *",
            "   *      * ",
            "  *     **  ",
            " *********  ",
            "            ",
            "            "
    };


    int opt;
    while (true) {
        cout << "Enter 0 (cleanup), 1 (warmup), 2 (simple), 3 (advanced) to select your algorithm, or other value to quit the program:" << endl;
        cin >> opt;
        if (opt < 0 || opt > 3)
            return 0;

        fill(opt, test_tube, 0, 5);
        fill(opt, conical_flask, 0, 6);
        fill(opt, flask,0,5);
        fill(opt, up_and_down, 0, 2);
        if (opt != ADVANCED_FILL)
            fill(opt, two_tubes, 0, 6);
        else
            cout << "Not for advanced_fill" << endl;
        if (opt != SIMPLE_FILL && opt != WARMUP_FILL)
            fill(opt, broken_flask, 0, 6);
        else
            cout << "Not for warmup_fill and simple_fill" << endl;
        if (opt != SIMPLE_FILL && opt != WARMUP_FILL)
            fill(opt, broken_tube, 0, 6);
        else
            cout << "Not for warmup_fill and simple_fill" << endl;
        if (opt != SIMPLE_FILL && opt != WARMUP_FILL)
            fill(opt, beaker, 0, 4);
        else
            cout << "Not for warmup_fill and simple_fill" << endl;
        if (opt != SIMPLE_FILL && opt != WARMUP_FILL)
            fill(opt, broken_u_flask,0,11);
        else
            cout << "Not for warmup_fill and simple_fill" << endl;
    }
    return 0;


}