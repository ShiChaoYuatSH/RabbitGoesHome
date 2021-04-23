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
    char adj; // bit 0 for rightwards and bit 1 for downwards; 1 represents forward available and 0 for blocked
};

// reset the status of the cell which is in cellIndex
void resetCellAdj(CELLFLAG* grid, Cell* paths, int cellIndex, int rows, int columns) {
    const int dstCell = rows * columns - 1;
    paths[cellIndex].adj = 0;
    int rightwards = paths[cellIndex].pos + 1;
    int downwards = paths[cellIndex].pos + columns;
    if(((rightwards % columns) != 0) && (rightwards <= dstCell) && (grid[rightwards] != SNAKE)) { // rightwards available
        paths[cellIndex].adj |= 0x01;
    }
    
    if((downwards <= dstCell) && (grid[downwards] != SNAKE)) { // downwards available
        paths[cellIndex].adj |= 0x02;
    }
}

int printAllPaths(const char* funcName, CELLFLAG* grid, int rows, int columns) {
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
        int rightwards = paths[curStep].pos + 1;
        int downwards  = paths[curStep].pos + columns;
        if(((rightwards % columns) != 0) && (rightwards <= dstCell) && (grid[rightwards] != SNAKE) && (vis[rightwards] == UNVISITED) && ((paths[curStep].adj & 0x01) == 1)) {
            paths[curStep].adj &= 0xFE;
            vis[paths[curStep].pos] = VISITED;
            // push rightwards cell into stack at curStep
            curStep++;
            paths[curStep].pos = rightwards;
            resetCellAdj(grid, paths, curStep, rows, columns);
        }
        else if((downwards <= dstCell) && (grid[downwards] != SNAKE) && (vis[downwards] == UNVISITED) && ((paths[curStep].adj & 0x02) != 0)) {
            paths[curStep].adj &= 0xFD;
            vis[paths[curStep].pos] = VISITED;
            // push downwards cell into stack at curStep
            curStep++;
            paths[curStep].pos = downwards;
            resetCellAdj(grid, paths, curStep, rows, columns);
        }
        else if((paths[curStep].pos == dstCell)) { // print path
            if((++numPaths) == 1) {
                std::cout << "paths of function " << funcName << ": " << std::endl;
            }
            for(int i = 0; i < curStep; i++) {
                std::cout << paths[i].pos << " -> ";
            }
            std::cout << paths[curStep].pos << std::endl;
            vis[paths[curStep].pos] = UNVISITED;
            resetCellAdj(grid, paths, curStep, rows, columns);
            curStep--; // pop from stack
        }
        else if(((paths[curStep].adj & 0x01) == 0) && ((paths[curStep].adj & 0x02) == 0)){
            vis[paths[curStep].pos] = UNVISITED;
            resetCellAdj(grid, paths, curStep, rows, columns);
            curStep--; // pop from stack
        }
        else {
            assert(false);
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
