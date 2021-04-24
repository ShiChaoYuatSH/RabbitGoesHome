// A rabbit is in the top-left-most cell of a M x N grid. The grid width is M-cell and the height is N-cell. 
// Some cells have snakes and other cells don’t. The rabbit wants to go home,
// which is located at the bottom-right-most cell of the grid.
// It can only move rightwards or downwards (no diagonal movement) and do so one cell at a time.
// The rabbit cannot move to a cell that has snakes.
// In this program the different paths will be printed.

#include<cassert>  // for assert
#include<iostream> // for cout

enum CELLFLAG {
    SNAKE,
    FLATLAND
};

enum VISITEDFLAG {
    UNVISITED,
    VISITED
};

struct Cell {
    int  pos;
    char adj; // bit 0 for rightwards and bit 1 for downwards; 1 represents forward available and 0 for blocked
};

constexpr int STEP = 1;

// Input
//     grid      : the pointer which points to the matrix by row major order
//     paths     : the pointer which points to the array which store the nodes in the path
//     cellIndex : the cell to be reset
//     rows      : the number of matrix rows
//     columns   : the number of matrix columns
// Output
//     available towards of paths[cellIndex].adj
// Synopsis
//     reset the avaliable towards(rightwards & downwards) of the cell which is in cellIndex
inline void resetCellAdj(CELLFLAG* grid, Cell* paths, int cellIndex, int rows, int columns) {
    const int dstCell = rows * columns - STEP;
    paths[cellIndex].adj = 0;
    int rightwards = paths[cellIndex].pos + STEP;
    int downwards = paths[cellIndex].pos + columns;
    if(((rightwards % columns) != 0) && (rightwards <= dstCell) && (grid[rightwards] != SNAKE)) { // rightwards available
        paths[cellIndex].adj |= 0x01;
    }
    
    if((downwards <= dstCell) && (grid[downwards] != SNAKE)) { // downwards available
        paths[cellIndex].adj |= 0x02;
    }
}

// Input
//     gridName  : the name of grid
//     grid      : the pointer which points to the matrix by row major order
//     rows      : the number of matrix rows
//     columns   : the number of matrix columns
// Output
//     number of correct paths
// Synopsis
//     print all paths from top-left-most cell to the bottom-right-most cell which move rightwards or downwards (no diagonal movement) and do so one cell at a time
int printAllPaths(const char* gridName, CELLFLAG* grid, int rows, int columns) {
    constexpr int EMPTYSTACK = -1;
    int numPaths = 0;
    const int dstCell = rows * columns - 1;
    VISITEDFLAG vis[rows * columns];
    for(int i = 0; i < (rows * columns); i++) {
        vis[i] = UNVISITED;
    }

    // treat paths as stack
    Cell paths[rows * columns];
    for(int i = 0; i < (rows * columns); i++) {
        paths[i].pos = 0;
        paths[i].adj = 0;
    }
    int curStep = EMPTYSTACK;
    
    // move to next position in stack
    curStep++;
    
    // push source cell into stack at curStep
    paths[curStep].pos = 0;
    resetCellAdj(grid, paths, 0, rows, columns);

    while(curStep >= 0) { // until stack empty
		const int curPos = paths[curStep].pos;
        const int rightwards = curPos + STEP;
        const int downwards  = curPos + columns;
        if((curPos == dstCell)) { // print path
            if((++numPaths) == 1) {
                std::cout << "paths of function " << gridName << ": " << std::endl;
            }
            for(int i = 0; i < curStep; i++) {
                std::cout << paths[i].pos << " -> ";
            }
            std::cout << paths[curStep].pos << std::endl;
            vis[curPos] = UNVISITED;
            resetCellAdj(grid, paths, curStep, rows, columns);
            curStep--; // pop from stack
        }
		// move rightwards
        else if(((rightwards % columns) != 0) && (rightwards <= dstCell) && (grid[rightwards] != SNAKE) && (vis[rightwards] == UNVISITED) && ((paths[curStep].adj & 0x01) == 1)) {
            paths[curStep].adj &= 0xFE;
            vis[curPos] = VISITED;
            // push rightwards cell into stack at curStep
            curStep++;
            paths[curStep].pos = rightwards;
            resetCellAdj(grid, paths, curStep, rows, columns);
        }
		// move downwards
        else if((downwards <= dstCell) && (grid[downwards] != SNAKE) && (vis[downwards] == UNVISITED) && ((paths[curStep].adj & 0x02) != 0)) {
            paths[curStep].adj &= 0xFD;
            vis[curPos] = VISITED;
            // push downwards cell into stack at curStep
            curStep++;
            paths[curStep].pos = downwards;
            resetCellAdj(grid, paths, curStep, rows, columns);
        }
        else { // blocked, backtrack
            vis[curPos] = UNVISITED;
            resetCellAdj(grid, paths, curStep, rows, columns);
            curStep--; // pop from stack
        }
    }
    
    return numPaths;
}

// This case will cover the 3*3 matrix with 3 snakes
void testZeroPath1() {
    constexpr int rows = 3;
    constexpr int cols = 3;
    constexpr int result = 0;

    CELLFLAG grid[rows][cols] = { {FLATLAND, FLATLAND, SNAKE}, {FLATLAND, SNAKE, FLATLAND}, {SNAKE, FLATLAND, FLATLAND} };
    assert(printAllPaths(__FUNCTION__, *grid, rows, cols) == result);
}

// This case will cover the 3*3 matrix with 2 snakes
void testZeroPath2() {
    constexpr int rows = 3;
    constexpr int cols = 3;
    constexpr int result = 0;

    CELLFLAG grid[rows][cols] = { {FLATLAND, SNAKE, FLATLAND}, {SNAKE, FLATLAND, FLATLAND}, {FLATLAND, FLATLAND, FLATLAND} };
    assert(printAllPaths(__FUNCTION__, *grid, rows, cols) == result);
}

// This case will cover the 3*3 matrix with 2 snakes
void testZeroPath3() {
    constexpr int rows = 3;
    constexpr int cols = 3;
    constexpr int result = 0;

    CELLFLAG grid[rows][cols] = { {FLATLAND, FLATLAND, FLATLAND}, {FLATLAND, FLATLAND, SNAKE}, {FLATLAND, SNAKE, FLATLAND} };
    assert(printAllPaths(__FUNCTION__, *grid, rows, cols) == result);
}

// This case will cover the 3*3 matrix with 2 snakes
void testOnePath() {
    constexpr int rows = 3;
    constexpr int cols = 3;
    constexpr int result = 1;

    CELLFLAG grid[rows][cols] = { {FLATLAND, FLATLAND, FLATLAND}, {FLATLAND, SNAKE, FLATLAND}, {SNAKE, FLATLAND, FLATLAND} };
    assert(printAllPaths(__FUNCTION__, *grid, rows, cols) == result);
}

// This case will cover the 3*3 matrix without snakes
void testFullPath() {
    constexpr int rows = 3;
    constexpr int cols = 3;
    constexpr int result = 6;

    CELLFLAG grid[rows][cols] = { {FLATLAND, FLATLAND, FLATLAND}, {FLATLAND, FLATLAND, FLATLAND}, {FLATLAND, FLATLAND, FLATLAND} };
    assert(printAllPaths(__FUNCTION__, *grid, rows, cols) == result);
}

// This case will cover the 2*3 matrix with 1 snake
void testNumPathOfRectMatrix1() {
    constexpr int rows = 2;
    constexpr int cols = 3;
    constexpr int result = 1;

    CELLFLAG grid[rows][cols] = { {FLATLAND, FLATLAND, FLATLAND}, {FLATLAND, SNAKE, FLATLAND} };
    assert(printAllPaths(__FUNCTION__, *grid, rows, cols) == result);
}

// This case will cover the 3*2 matrix with 1 snake
void testNumPathOfRectMatrix2() {
    constexpr int rows = 3;
    constexpr int cols = 2;
    constexpr int result = 1;

    CELLFLAG grid[rows][cols] = { {FLATLAND, FLATLAND}, {SNAKE, FLATLAND}, {FLATLAND, FLATLAND} };
    assert(printAllPaths(__FUNCTION__, *grid, rows, cols) == result);
}

int main() {
    testZeroPath1(); // 0 path expected
    testZeroPath2(); // 0 path expected
    testZeroPath3(); // 0 path expected
    testOnePath();   // 1 path expected
    testFullPath();  // 6 paths expected
    testNumPathOfRectMatrix1(); // 1 path expected
    testNumPathOfRectMatrix2(); // 1 path expected
}
