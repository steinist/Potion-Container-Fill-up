#include <fstream>
#include <string>
#include <iostream>
#include <cstring>

//PASTE YOUR CODE HERE, EXCEPT main() 
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


//// Punya Stefan
//int advanced_fill(char array[][ARRAY_SIZE+1], int row, int col) {
//// Don't forget to return!
//    int rim_level = find_level(array);
//
//    array[row + rim_level][col] = POTION;
//
//
//    if (is_spill(array)) {
//        cleanup(array);
//    }
//
////    print_array(array);
//        //Down
//
//        if (array[row + rim_level][col] == POTION && row + rim_level + 1 < ARRAY_SIZE && array[row + rim_level + 1][col] == EMPTY) {
//            advanced_fill(array, row + 1, col);
//        }
//        //Right
//        if (array[row + rim_level][col] == POTION && col + 1 < ARRAY_SIZE && array[row + rim_level][col + 1] == EMPTY) {
//            advanced_fill(array, row, col + 1);
//        }
//        //Left
//        if (array[row + rim_level][col] == POTION && col - 1 > 0 && array[row + rim_level][col - 1] == EMPTY) {
//            advanced_fill(array, row, col - 1);
//        }
//        //Up
//        if (array[row + rim_level][col] == POTION && row - 1 >= 0 && array[row + rim_level - 1][col] == EMPTY) {
//            advanced_fill(array, row - 1, col);
//        }
//
//}


//int advanced_fill(char array[][ARRAY_SIZE+1], int row, int col) {
//// Don't forget to return!
//    int rim_level = find_level(array, col);
//    cout << "level: " << rim_level << endl;
//
//    warmup_fill2(array, row, col, rim_level);
//
//    bool spill = is_spill1(array);
//
//    if (spill){
//        cout << "SPILL !!!" << endl;
//        return cleanup(array);
//    }
//}


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



//////


#include "map.h"
using namespace std;
bool DEBUG = true;
bool check_value(const char array[][ARRAY_SIZE + 1], const char sol[][ARRAY_SIZE + 1]) {
    for (int i = 0; i < ARRAY_SIZE; i++) {
	for (int j = 0; j < ARRAY_SIZE; j++) 
	    if (array[i][j] != sol[i][j]) return false;
	if (array[i][ARRAY_SIZE] != '\0') return false;
    }
    return true;
}

void print_array_2(const char array[][ARRAY_SIZE+1]) {
    cout << "  ";
    for (int j = 0; j < ARRAY_SIZE; j++)
	cout << j % 10;
    cout << endl;
    for (int i = 0; i < ARRAY_SIZE + 2; i++) {
	cout << i % 10 << " ";
	if (i == 0 || i == ARRAY_SIZE + 1)
	   cout << array[i] << "  <-- out-of-bound array content, should not be changed" << endl;
	else
	   cout << array[i] << endl;
    }
}


void copy(char dst[][ARRAY_SIZE+1], const char src[][ARRAY_SIZE+1]) {
    for (int i = 0; i < ARRAY_SIZE; i++)
	for (int j = 0; j < ARRAY_SIZE + 1; j++)
	    dst[i][j] = src[i][j];
}
// cleanup
//
// There are few possible errors a cleanup can have
// 1. unfinished cleanup or over-clean (remove original WALL and RIM)
// 2. return wrong cleanup count
// 3. clean array[ARRAY_SIZE] or beyond
//
const int WRONG_ARRAY = 4;
const int WRONG_COUNT = 2;
const int WRONG_BOUND = 1;
const int CORRECT = 0;

int ta_cleanup(char [][ARRAY_SIZE + 1], int=0, int=0);
int compare_cleanup(const char a[][ARRAY_SIZE + 1]) {
    char x[ARRAY_SIZE][ARRAY_SIZE+1];
    char y[ARRAY_SIZE+2][ARRAY_SIZE+1] = {};
    copy(x, a);
    copy(&y[1], a);  // y[0] and y[ARRAY_SIZE + 1] is out-of-bound memory
    strcpy(y[ARRAY_SIZE + 1], "oooooooooooo");  //you should not touch this
    strcpy(y[0],              "oooooooooooo");   //you should not touch this

    int student = cleanup(&y[1]);
    int ta = ta_cleanup(x);

    int result = CORRECT;
    if (DEBUG) {
	cout << " Student Cleanup. return value=" << student << endl;
	print_array_2(y);
	cout << " Solution Cleanup. return value=" << ta << endl;
	print_array(x);
    }


    if (!check_value(x, &y[1])) result |= WRONG_ARRAY;
    if (ta != student) result |= WRONG_COUNT;
    for (int i = 0; i < ARRAY_SIZE; i++)
	if (y[ARRAY_SIZE+1][i] != 'o' || y[0][i] != 'o') 
	    result |= WRONG_BOUND;

    return result;

}


int test0() {    return compare_cleanup(f_bowl);    }
int test1() {    return compare_cleanup(f_cup);    }
int test2() {    return compare_cleanup(f_two_tubes);    }
int test3() {    return compare_cleanup(f_conical_flask);    }
int test4() {    return compare_cleanup(two_tubes);    }


//warmup_fill
//
//
void ta_warmup_fill(char [][ARRAY_SIZE + 1], int, int, int);
int compare_warmup(const char a[][ARRAY_SIZE+1], const int col, const int level) {
    char x[ARRAY_SIZE][ARRAY_SIZE+1];   
    char y[ARRAY_SIZE][ARRAY_SIZE+1];
    copy(x, a);
    copy(y, a);
    ta_warmup_fill(x, 0, col, level);
    warmup_fill(y, 0, col, level);

    int result = CORRECT;

    if (DEBUG) {
	cout << " Student warmup. " <<  endl << "drop here" << endl;
	for (int i = 0; i < col+2; i++)
	    cout << ' ';
	cout << 'D' << endl;
	print_array(x);

	cout << " Solution warmup. " << endl << "drop here" << endl;
	for (int i = 0; i < col+2; i++)
	    cout << ' ';
	cout << 'D' << endl;
	print_array(y);
    }


    if (!check_value(x, y)) result |= WRONG_ARRAY;
    return result;
}

int test5() {    return compare_warmup(two_tubes, 6, 1);}
int test6() {    return compare_warmup(test_tube, 6, 4);}
int test7() {    return compare_warmup(conical_flask, 6, 4);}
int test8() {    return compare_warmup(flask, 5, 1);}
int test9() {    return compare_warmup(up_and_down, 2, 4);}
int test10() {    return compare_warmup(trio, 6, 4);}
int test11() {    return compare_warmup(wu_tong, 6, 4);}
int test12() {    return compare_warmup(rocket, 6, 4);}

//simple_fill
//
//
void ta_simple_fill(char [][ARRAY_SIZE + 1], int, int);
int compare_simple(const char a[][ARRAY_SIZE+1], const int col) {
    char x[ARRAY_SIZE][ARRAY_SIZE+1];
    char y[ARRAY_SIZE][ARRAY_SIZE+1];
    copy(x, a);
    copy(y, a);
    ta_simple_fill(x, 0, col);
    simple_fill(y, 0, col);

    int result = CORRECT;

    if (DEBUG) {
	cout << " Student simple. " <<  endl << "drop here" << endl;
	for (int i = 0; i < col+2; i++)
	    cout << ' ';
	cout << 'D' << endl;
	print_array(x);

	cout << " Solution simple. " << endl << "drop here" << endl;
	for (int i = 0; i < col+2; i++)
	    cout << ' ';
	cout << 'D' << endl;
	print_array(y);
    }


    if (!check_value(x, y)) result |= WRONG_ARRAY;
    return result;
}

int test13() { return compare_simple(two_tubes, 6); }
int test14() {    return compare_simple(test_tube, 6);}
int test15() {    return compare_simple(conical_flask, 6);}
int test16() {    return compare_simple(flask, 5);}
int test17() {    return compare_simple(up_and_down, 2);}
int test18() {    return compare_simple(trio, 6);}
int test19() {    return compare_simple(wu_tong, 6);}
int test20() {    return compare_simple(rocket, 6);}


//advanced_fill
//
int ta_advanced_fill(char [][ARRAY_SIZE + 1], int, int);
int compare_adv(const char a[][ARRAY_SIZE+1], const int col) {
    char x[ARRAY_SIZE][ARRAY_SIZE+1];
    char y[ARRAY_SIZE+2][ARRAY_SIZE+1] = {};
    copy(x, a);
    copy(&y[1], a);
    strcpy(y[ARRAY_SIZE + 1], "************");  //you should not touch this
    strcpy(y[0],              "TTTTTTTTTTTT");   //you should not touch this
    ta_advanced_fill(x, 0, col);
    advanced_fill(&y[1], 0, col);

    int result = CORRECT;

    if (DEBUG) {
	
	cout << " Student advanced. " <<  endl << "drop here" << endl;
	for (int i = 0; i < col+2; i++)
	    cout << ' ';
	cout << 'D' << endl;
	print_array_2(y);

	cout << " Solution advanced. " << endl << "drop here" << endl;
	for (int i = 0; i < col+2; i++)
	    cout << ' ';
	cout << 'D' << endl;
	print_array(x);
    }


    if (!check_value(x, &y[1])) result |= WRONG_ARRAY;
    for (int i = 0; i < ARRAY_SIZE; i++)
	if (y[ARRAY_SIZE+1][i] != '*' || y[0][i] != 'T') 
	    result |= WRONG_BOUND;

    return result;
}

int test21() { return compare_adv(test_tube, 6); }
int test22() { return compare_adv(test_tube, 1); }
int test23() { return compare_adv(beaker, 3); }
int test24() { return compare_adv(beaker, 6); }
int test25() { return compare_adv(broken_flask, 6); }
int test26() { return compare_adv(broken_u_flask, 5); }
int test27() { return compare_adv(broken_u_flask, 10); }
int test28() { return compare_adv(up_and_down, 2); }
int test29() { return compare_adv(side_broken, 1); }
int test30() { return compare_adv(conical_flask, 5); } 
int test31() { return compare_adv(ripple, 2); } 
int test32() { return compare_adv(ripple, 5); }
int test33() { return compare_adv(blank, 1); }
int test34() { return compare_adv(timer, 3); }
int test35() { return compare_adv(flask, 7); }
int test36() { return compare_adv(flask, 6); }
int main(int argc, char* argv[]) {
    int  (*f[])() = {
	test0, test1, test2, test3, test4,
	test5, test6, test7, test8, test9, test10, test11, test12,
	test13,test14,test15,test16,test17,test18,test19,test20,
	test21,test22,test23,test24,test25,test26,test27,test28,test29,test30,test31,test32,test33,test34,test35,test36

    }; 
    const int TOTAL_CASES = sizeof(f)/sizeof(f[0]);
    if (argc == 2) { 
	int test_case = atoi(argv[1]);
	DEBUG = false;
	if (test_case >= TOTAL_CASES || test_case < 0) 
	    cout << "No such test case" << endl;
	else {
	    ofstream ofs(string(argv[1]) + ".log");
	    ofs << f[test_case]() << endl;
	    ofs.close();
	}
    } else {
	cout << "Please enter the number of test cases you want to test" << endl;
	cout << "[0-" << TOTAL_CASES - 1 << "]" << endl;
	int test_case;
	cin >> test_case;
	if (test_case >= TOTAL_CASES || test_case < 0) 
	    cout << "No such test case" << endl;
	else {
	    int result = f[test_case]();
	    cout << endl << "Test Result (non zero = incorrect):" << result << endl;
	}
    }

    return 0;
}




