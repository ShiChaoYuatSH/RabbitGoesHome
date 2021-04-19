// A rabbit is in the top-left-most cell of a M x N grid. The grid width is M-cell and the height is N-cell. 
// Some cells have snakes and other cells don’t. The rabbit wants to go home,
// which is located at the bottom-right-most cell of the grid.
// It can only move rightwards or downwards (no diagonal movement) and do so one cell at a time.
// The rabbit cannot move to a cell that has snakes.
// In this program the different paths will be counted by dynamic programming

#include<cassert> // for assert function

enum CELLFLAG {
    SNAKE,
    FLATLAND
};

// Input
//     grid    : the pointer which points to the matrix by row major order
//     rows    : the number of matrix rows
//     columns : the number of matrix columns
// Output
//     the number of different paths from top-left-most cell to the bottom-right-most cell of the grid
// Synopsis
//     The total number of different paths is counted by the dynamic programming:
//     dp[i][j] = dp[i-1][j] + dp[i][j-1]
int countAllPaths(CELLFLAG* grid, int rows, int columns) {
    assert(((grid != nullptr) && ((rows * columns) != 0)) || ((grid == nullptr) && ((rows * columns) == 0)));
    assert(grid[0] == FLATLAND);
    assert(grid[rows * columns - 1] == FLATLAND);

    //grid to store the number of paths of every cell
    int num_paths[rows * columns];
    for(int i = 0; i < (rows * columns); i++) {
        num_paths[i] = 0;
    }
    
    num_paths[0] = grid[0];
    for(int i = 1; i < rows; i++) { // initialize the left-most column
        if((num_paths[(i - 1) * columns] == 1) && (grid[i * columns] == FLATLAND)) {
            num_paths[i * columns] = 1;
        }
    }
    
    for(int i = 1; i < columns; i++) { // initialize the top-most column
        if((num_paths[i - 1] == 1) && (grid[i] == FLATLAND)) {
            num_paths[i] = 1;
        }
    }
    
    // traverse the matrix until the destination
    for(int i = 1; i < rows; i++) {
        for(int j = 1; j < columns; j++) {
            if(grid[i * columns + j] == FLATLAND) {
                num_paths[i * columns + j] = num_paths[(i - 1) * columns + j] + num_paths[i * columns + j - 1];
            }
        }
    }
    
    return num_paths[rows * columns - 1];
}

// This case will cover the 3*3 matrix with 3 snakes
void testZeroPath1() {
    constexpr int rows = 3;
    constexpr int cols = 3;
    constexpr int result = 0;

    CELLFLAG grid[rows][cols] = { {FLATLAND, FLATLAND, SNAKE}, {FLATLAND, SNAKE, FLATLAND}, {SNAKE, FLATLAND, FLATLAND} };
    assert(countAllPaths(*grid, rows, cols) == result);
}

// This case will cover the 3*3 matrix with 2 snakes
void testZeroPath2() {
    constexpr int rows = 3;
    constexpr int cols = 3;
    constexpr int result = 0;

    CELLFLAG grid[rows][cols] = { {FLATLAND, SNAKE, FLATLAND}, {SNAKE, FLATLAND, FLATLAND}, {FLATLAND, FLATLAND, FLATLAND} };
    assert(countAllPaths(*grid, rows, cols) == result);
}

// This case will cover the 3*3 matrix with 2 snakes
void testZeroPath3() {
    constexpr int rows = 3;
    constexpr int cols = 3;
    constexpr int result = 0;

    CELLFLAG grid[rows][cols] = { {FLATLAND, FLATLAND, FLATLAND}, {FLATLAND, FLATLAND, SNAKE}, {FLATLAND, SNAKE, FLATLAND} };
    assert(countAllPaths(*grid, rows, cols) == result);
}

// This case will cover the 3*3 matrix with 2 snakes
void testOnePath() {
    constexpr int rows = 3;
    constexpr int cols = 3;
    constexpr int result = 1;

    CELLFLAG grid[rows][cols] = { {FLATLAND, FLATLAND, FLATLAND}, {FLATLAND, SNAKE, FLATLAND}, {SNAKE, FLATLAND, FLATLAND} };
    assert(countAllPaths(*grid, rows, cols) == result);
}

// This case will cover the 3*3 matrix without snakes
void testFullPath() {
    constexpr int rows = 3;
    constexpr int cols = 3;
    constexpr int result = 6;

    CELLFLAG grid[rows][cols] = { {FLATLAND, FLATLAND, FLATLAND}, {FLATLAND, FLATLAND, FLATLAND}, {FLATLAND, FLATLAND, FLATLAND} };
    assert(countAllPaths(*grid, rows, cols) == result);
}

// This case will cover the 2*3 matrix with 1 snake
void testNumPathOfRectMatrix1() {
    constexpr int rows = 2;
    constexpr int cols = 3;
    constexpr int result = 1;

    CELLFLAG grid[rows][cols] = { {FLATLAND, FLATLAND, FLATLAND}, {FLATLAND, SNAKE, FLATLAND} };
    assert(countAllPaths(*grid, rows, cols) == result);
}

// This case will cover the 3*2 matrix with 1 snake
void testNumPathOfRectMatrix2() {
    constexpr int rows = 3;
    constexpr int cols = 2;
    constexpr int result = 1;

    CELLFLAG grid[rows][cols] = { {FLATLAND, FLATLAND}, {SNAKE, FLATLAND}, {FLATLAND, FLATLAND} };
    assert(countAllPaths(*grid, rows, cols) == result);
}

int main() {
    testZeroPath1(); // 0 path expected
    testZeroPath2(); // 0 path expected
    testZeroPath3(); // 0 path expected
    testOnePath();   // 1 path expected
    testFullPath();  // 6 paths expected
    testNumPathOfRectMatrix1(); // 1 path expected
    testNumPathOfRectMatrix2(); // 1 path expected
    return 0;
}
