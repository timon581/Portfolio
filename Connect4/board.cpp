#include <iostream>
#include "board.h"

#ifndef CHECK
#define CHECKMSG(condition, text) \
if(!(condition)) {fprintf(stderr,"file: " __FILE__ "\nline: %d\nmsg:  " text "\n",__LINE__); exit(1);}
#define CHECK(condition) \
if(!(condition)) {fprintf(stderr,"Assertion failed!\nfile: " __FILE__ "\nline: %d\n" ,__LINE__); exit(1);}
#endif

void Board::Initialize() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            field[i][j] = EMPTY;
        }
    }
    for (int i = 0; i < cols; ++i) {
        height[i] = 0;
    }
}

Board::Board() : LastMover(EMPTY), lastcol(-1) {
    Initialize();
}

Board::Board(const Board &src) {
    LastMover = src.LastMover;
    lastcol = src.lastcol;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            field[i][j] = src.field[i][j];
        }
    }
    for (int i = 0; i < cols; ++i) {
        height[i] = src.height[i];
    }
}

bool Board::MoveLegal(const int col) {
    assert(col >= 0 && col < cols);
    return height[col] < rows;
}

bool Board::Move(const int col, const player player) {
    if (!MoveLegal(col))
        return false;
    field[height[col]][col] = player;
    height[col]++;
    LastMover = player;
    lastcol = col;
    return true;
}

bool Board::UndoMove(const int col) {
    assert(col >= 0 && col < cols);
    if (height[col] == 0)
        return false;
    height[col]--;
    field[height[col]][col] = EMPTY;
    return true;
}

bool Board::is_full() {
    for (int col = 0; col < cols; col++) {
        if (field[0][col] == EMPTY) {
            return false;
        }
    }
    return true;
}

bool Board::GameEnd(const int lastcol) {
    int seq, player, row, col, r, c;
    assert(lastcol >= 0 && lastcol < cols);
    col = lastcol;
    row = height[lastcol] - 1;
    if (row < 0)
        return false;
    player = field[row][col];

    seq = 1;
    r = row - 1;
    while (r >= 0 && field[r][col] == player) {
        seq++;
        r--;
    }
    if (seq > 3)
        return true;

    seq = 0;
    c = col;
    while ((c - 1) >= 0 && field[row][c - 1] == player)
        c--;
    while (c < cols && field[row][c] == player) {
        seq++;
        c++;
    }
    if (seq > 3)
        return true;

	seq = 0;
	r = row;
	c = col;
	while ((c - 1) >= 0 && (r - 1) >= 0 && field[r - 1][c - 1] == player) {
		c--;
		r--;
	}
	while (c < cols && r < rows && field[r][c] == player) {
		seq++;
		c++;
		r++;
	}
	if (seq > 3)
		return true;

	seq = 0;
	r = row;
	c = col;
	while ((c - 1) >= 0 && (r + 1) < rows && field[r + 1][c - 1] == player) {
		c--;
		r++;
	}
	while (c < cols && r >= 0 && field[r][c] == player) {
		seq++;
		c++;
		r--;
	}
	if (seq > 3)
		return true;

	return false;
}

void Board::printBoard() const {
    for(int i=rows-1;i>=0;i--)
    {	for(int j=0;j<cols;j++)
            std::cout << field[i][j] << " | ";
       std::cout << "\n";
    }
    std::cout << "\n";
}

player Board::getLastMover() const {
    return LastMover;
}

