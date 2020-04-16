/*
 * main.cpp
 *
 *  Created on: Sep 19, 2019
 *      Author: kevinw
 */

#include <iostream>
#include <cstring>
#include <string>
using namespace std;

const int TA_ARRAY_SIZE = 12;
const char TA_POTION = 'o';
const char TA_RIM = 'T';
const char TA_WALL = '*';
const char TA_EMPTY = ' ';

enum {SPILL = -1, KEEP_FILLING, FULL};



int ta_cleanup(char array[TA_ARRAY_SIZE][TA_ARRAY_SIZE+1], int row = 0, int col = 0) {
    if (row < 0 || row >= TA_ARRAY_SIZE || col < 0 || col >= TA_ARRAY_SIZE)
	return 0;

    int sum = 0;
    if (array[row][col] == TA_POTION) {
	array[row][col] = TA_EMPTY;
	sum = 1;
    }
    if (col == 0)
	sum += ta_cleanup(array, row + 1, col);
    sum += ta_cleanup(array, row, col + 1);
    return sum;

}

void ta_warmup_fill(char array[][TA_ARRAY_SIZE+1], int row, int col, int level) {
    if (row < 0 || row >= TA_ARRAY_SIZE || col < 0 || col >= TA_ARRAY_SIZE)
	return;
    switch (array[row][col]) {
    case TA_WALL:
    case TA_RIM:
	return;
    case TA_EMPTY:
	if (level <= row) {
	    array[row][col] = TA_POTION;
	    ta_warmup_fill(array, row, col + 1, level);
	    ta_warmup_fill(array, row, col - 1, level);
	    if (level < row)
		ta_warmup_fill(array, row - 1, col, level);
	}
	ta_warmup_fill(array, row + 1, col, level);
    }
}


void ta_simple_fill(char array[][TA_ARRAY_SIZE+1], int row, int col) {
    if (row < 0 || row >= TA_ARRAY_SIZE || col < 0 || col >= TA_ARRAY_SIZE)
	return;
    switch (array[row][col]) {
    case TA_WALL:
    case TA_RIM:
	return;
    case TA_EMPTY:
	if (row == TA_ARRAY_SIZE - 1) //reach the bottom, spilling out
	    return;
	if (array[row + 1][col] == TA_EMPTY) {//water drop down
	    ta_simple_fill(array, row + 1, col);
	    break;
	} else
	    array[row][col] = TA_POTION;
	// no break; and go into the water case
    case TA_POTION:
	if ((col > 0 && array[row][col - 1] == TA_RIM) ||
		(col < TA_ARRAY_SIZE - 1 && array[row][col + 1] == TA_RIM))
	    return;
	if (col > 0 && array[row][col - 1] == TA_EMPTY)
	    ta_simple_fill(array, row, col - 1);
	if (col < TA_ARRAY_SIZE - 1 && array[row][col + 1] == TA_EMPTY)
	    ta_simple_fill(array, row, col + 1);
	ta_simple_fill(array, row - 1, col);
    }
}


//A helper function that checks its left or right is a RIM or a WALL or OUT-OF-BOUND
int ta_check_bound(const char array[][TA_ARRAY_SIZE+1], int row, int col, bool left) {
    if (row < 0 || row >= TA_ARRAY_SIZE || col < 0 || col >= TA_ARRAY_SIZE) {
	return SPILL;
    }
    switch (array[row][col]) {
    case TA_WALL: return KEEP_FILLING;
    case TA_RIM: return FULL;
    }
    return ta_check_bound(array, row, col + (left ? -1 : 1), left);
}


int ta_advanced_fill(char array[][TA_ARRAY_SIZE+1], int row, int col) {
    if (row < 0 || row >= TA_ARRAY_SIZE || col < 0 || col >= TA_ARRAY_SIZE) {
	return SPILL;
    }
    int state = KEEP_FILLING;
    int state_left = KEEP_FILLING, state_right = KEEP_FILLING;
    switch (array[row][col]) {
    case TA_WALL:return KEEP_FILLING;
    case TA_RIM: return FULL;
    case TA_EMPTY:
	if (row == TA_ARRAY_SIZE - 1) //reach the bottom. Spilling out
	    return SPILL;
	else if (array[row + 1][col] == TA_EMPTY) {//water drop down
	    state = ta_advanced_fill(array, row + 1, col);
	} else {
	    array[row][col] = TA_POTION;
	    state = ta_advanced_fill(array, row, col);
	}
	if (state == SPILL) {
	    array[row][col] = TA_EMPTY;
	    ta_cleanup(array);
	}
	return state;
    case TA_POTION:
	if (col == 0 || col == TA_ARRAY_SIZE - 1)
	    return SPILL;
	if (array[row][col - 1] != TA_POTION)
	    state_left = ta_advanced_fill(array, row, col - 1);
	if (state_left == SPILL) {
	    return SPILL;
	}
	if (array[row][col + 1] != TA_POTION)
	    state_right = ta_advanced_fill(array, row, col + 1);
	if (state_right == SPILL) {
	    return SPILL;
	}
	//it is a full case except for one special condition
	//    *oooT  T
	//    *ooo****
	//    ********
	state_left = ta_check_bound(array, row, col, true);
	state_right = ta_check_bound(array, row, col, false);
	if (state_left == FULL || state_right == FULL) {
	    if (state_left != state_right)
		return SPILL;
	    else
		return FULL;
	}
	return ta_advanced_fill(array, row - 1, col);
    default:
	return SPILL; //impossible
    }

}



