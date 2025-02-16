#include <assert.h>
#include <iostream>

typedef int player;
const player EMPTY = 0;
const player CPU = 1;
const player HUMAN = 2;

class Board
{
private:
    static const int rows = 6;
    static const int cols = 7;
    player field[rows][cols]; // Static 6x7 array
    int height[cols];         // Array to track the height of each column
    player LastMover;
    int lastcol;
    void Initialize(); // Initialize and fill with EMPTY
public:
    bool is_full();
    Board();
    Board(const Board &src);
    ~Board() = default; // Destructor no longer needs to free memory
    int Columns() const { return cols; }
    player* operator[](const int row);
    bool MoveLegal(const int col);
    bool Move(const int col, const player player);
    bool UndoMove(const int col);
    bool GameEnd(const int lastcol);
    bool Load(int colNum, int rowNum);
    void Save(const char* fname);
    void printBoard() const;
    player getLastMover() const;
};

inline player* Board::operator[](const int row) {
    assert(row >= 0 && row < rows);
    return field[row];
}
