// A rabbit is in the top-left-most cell of a M x N grid. The grid width is M-cell and the height is N-cell. 
// Some cells have snakes and other cells don’t. The rabbit wants to go home,
// which is located at the bottom-right-most cell of the grid.
// It can only move up/down/left/right, but cannot revisit a cell it has already visited, and do so one cell at a time.
// The rabbit cannot move to a cell that has snakes.
// In this program the different paths will be printed.

#include<cassert>
#include<iostream>

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
	// bit 0 for rightwards, bit 1 for downwards, bit 2 for leftwards, bit 3 for upwards; 1 represents forward available and 0 for blocked
    char adj;
};

// Input
//     grid      : the pointer which points to the matrix by row major order
//     paths     : the pointer which points to the array which store the nodes in the path
//     cellIndex : the cell to be reset
//     rows      : the number of matrix rows
//     columns   : the number of matrix columns
// Output
//     available towards of paths[cellIndex].adj
// Synopsis
//     reset the avaliable towards(rightwards & downwards & leftwards & upwards) of the cell which is in cellIndex
inline void resetCellAdj(CELLFLAG* grid, Cell* paths, int cellIndex, int rows, int columns) {
    const int dstCell = rows * columns - 1;
    paths[cellIndex].adj = 0;
	const int curPos = paths[cellIndex].pos;
    int rightwards = curPos + 1;
    int downwards = curPos + columns;
	int leftwards = curPos - 1;
    int upwards = curPos - columns;
	
    if(((rightwards % columns) != 0) && (rightwards <= dstCell) && (grid[rightwards] != SNAKE)) { // rightwards available
        paths[cellIndex].adj |= 0x01;
    }
    
    if((downwards <= dstCell) && (grid[downwards] != SNAKE)) { // downwards available
        paths[cellIndex].adj |= 0x02;
    }
	
	if(((curPos % columns) != 0) && (leftwards >= 0) && (grid[leftwards] != SNAKE)) { // leftwards available
		paths[cellIndex].adj |= 0x04;
	}
	
	if((upwards >= 0) && (grid[upwards] != SNAKE)) { // upwards available
		paths[cellIndex].adj |= 0x08;
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
//     print all paths from top-left-most cell to the bottom-right-most cell which move up/down/left/right, but cannot revisit a cell it has already visited, and do so one cell at a time
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
		const int curPos     = paths[curStep].pos;
        const int rightwards = curPos + 1;
        const int downwards  = curPos + columns;
        const int leftwards  = curPos - 1;
        const int upwards    = curPos - columns;
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
		// move leftwards
        else if(((curPos % columns) != 0) && (leftwards <= dstCell) && (grid[leftwards] != SNAKE) && (vis[leftwards] == UNVISITED) && ((paths[curStep].adj & 0x04) != 0)) {
            paths[curStep].adj &= 0xFB;
            vis[curPos] = VISITED;
            // push leftwards cell into stack at curStep
            curStep++;
            paths[curStep].pos = leftwards;
            resetCellAdj(grid, paths, curStep, rows, columns);
        }
		// move upwards
        else if((upwards <= dstCell) && (grid[upwards] != SNAKE) && (vis[upwards] == UNVISITED) && ((paths[curStep].adj & 0x08) != 0)) {
            paths[curStep].adj &= 0xF7;
            vis[curPos] = VISITED;
            // push upwards cell into stack at curStep
            curStep++;
            paths[curStep].pos = upwards;
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

    CELLFLAG grid[rows][cols] = { {FLATLAND, FLATLAND, SNAKE}, {FLATLAND, SNAKE, FLATLAND}, {SNAKE, FLATLAND, FLATLAND} };
	int numPaths = printAllPaths(__FUNCTION__, *grid, rows, cols);
    std::cout << "test case " << __FUNCTION__ << " total path number: " << numPaths << std::endl;
}

// This case will cover the 3*3 matrix with 2 snakes
void testZeroPath2() {
    constexpr int rows = 3;
    constexpr int cols = 3;

    CELLFLAG grid[rows][cols] = { {FLATLAND, SNAKE, FLATLAND}, {SNAKE, FLATLAND, FLATLAND}, {FLATLAND, FLATLAND, FLATLAND} };
	int numPaths = printAllPaths(__FUNCTION__, *grid, rows, cols);
    std::cout << "test case " << __FUNCTION__ << " total path number: " << numPaths << std::endl;
}

// This case will cover the 3*3 matrix with 2 snakes
void testZeroPath3() {
    constexpr int rows = 3;
    constexpr int cols = 3;

    CELLFLAG grid[rows][cols] = { {FLATLAND, FLATLAND, FLATLAND}, {FLATLAND, FLATLAND, SNAKE}, {FLATLAND, SNAKE, FLATLAND} };
	int numPaths = printAllPaths(__FUNCTION__, *grid, rows, cols);
    std::cout << "test case " << __FUNCTION__ << " total path number: " << numPaths << std::endl;
}

// This case will cover the 3*3 matrix with 2 snakes
void testOnePath() {
    constexpr int rows = 3;
    constexpr int cols = 3;

    CELLFLAG grid[rows][cols] = { {FLATLAND, FLATLAND, FLATLAND}, {FLATLAND, SNAKE, FLATLAND}, {SNAKE, FLATLAND, FLATLAND} };
	int numPaths = printAllPaths(__FUNCTION__, *grid, rows, cols);
    std::cout << "test case " << __FUNCTION__ << " total path number: " << numPaths << std::endl;
}

// This case will cover the 3*3 matrix without snakes
void testFullPath() {
    constexpr int rows = 3;
    constexpr int cols = 3;

    CELLFLAG grid[rows][cols] = { {FLATLAND, FLATLAND, FLATLAND}, {FLATLAND, FLATLAND, FLATLAND}, {FLATLAND, FLATLAND, FLATLAND} };
	int numPaths = printAllPaths(__FUNCTION__, *grid, rows, cols);
    std::cout << "test case " << __FUNCTION__ << " total path number: " << numPaths << std::endl;
}

// This case will cover the 2*3 matrix with 1 snake
void testNumPathOfRectMatrix1() {
    constexpr int rows = 2;
    constexpr int cols = 3;

    CELLFLAG grid[rows][cols] = { {FLATLAND, FLATLAND, FLATLAND}, {FLATLAND, SNAKE, FLATLAND} };
	int numPaths = printAllPaths(__FUNCTION__, *grid, rows, cols);
    std::cout << "test case " << __FUNCTION__ << " total path number: " << numPaths << std::endl;
}

// This case will cover the 3*2 matrix with 1 snake
void testNumPathOfRectMatrix2() {
    constexpr int rows = 3;
    constexpr int cols = 2;

    CELLFLAG grid[rows][cols] = { {FLATLAND, FLATLAND}, {SNAKE, FLATLAND}, {FLATLAND, FLATLAND} };
	int numPaths = printAllPaths(__FUNCTION__, *grid, rows, cols);
    std::cout << "test case " << __FUNCTION__ << " total path number: " << numPaths << std::endl;
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
