// A rabbit is in the top-left-most cell of a M x N grid. The grid width is M-cell and the height is N-cell. 
// Some cells have snakes and other cells don’t. The rabbit wants to go home,
// which is located at the bottom-right-most cell of the grid.
// It can only move rightwards or downwards (no diagonal movement) and do so one cell at a time.
// The rabbit cannot move to a cell that has snakes.
// In this program the different paths will be counted with K number of cells have snakes.

#include<iostream>
#include<utility> // for std::pair
#include<cassert> // for assert

typedef std::pair<int, int> Position;
enum CELLFLAG {
    SNAKE,
    FLATLAND
};

// Input
//     snakes      : the pointer which points to the Position of snakes, this Position starts from 0
//     num_snakes  : the number of snakes
//     first       : position's first
//     second      : position's second
// Output
//     Position(first, second) has snake or not
// Synopsis
//     traverse the snake array to determine the position(first, second) has snake or not
inline CELLFLAG isSnake(Position* snakes, int num_snakes, int first, int second) {
    if(snakes == nullptr) return FLATLAND;
    
    for(int k = 0; k < num_snakes; k++) {
        if((snakes[k].first == first) && (snakes[k].second == second)) {
            return SNAKE;
        }
    }
    return FLATLAND;
}

// Input
//     snakes      : the pointer which points to the Position of snakes, this Position starts from 0
//     num_snakes  : the number of snakes
//     major_order : min(rows, columns)
//     minor_order : max(rows, columns)
// Output
//     the number of different paths from top-left-most cell to the bottom-right-most cell of the grid
// Synopsis
//     The total number of different paths is counted by iterative calculation
int countAllPathsByStride(Position* snakes, int num_snakes, int major_order, int minor_order) {
    int num_paths[major_order]; // store the intermediate result

    num_paths[0] = 1;
    for(int i = 1; i < major_order; i++) { // process the first stride
        if((num_paths[i - 1] == 1) && (isSnake(snakes, num_snakes, i, 0) == FLATLAND)) {
            num_paths[i] = 1;
        }
    }

    for(int j = 1; j < minor_order; j++) {
        // process the first cell
        if((num_paths[0] == 1) && (isSnake(snakes, num_snakes, 0, j) == FLATLAND)) {
            num_paths[0] = 1;
        }
        else {
            num_paths[0] = 0;
        }
        
        for(int i = 1; i < major_order; i++) {
            if(isSnake(snakes, num_snakes, i, j) == FLATLAND) {
                num_paths[i] += num_paths[i - 1];
            }
            else {
                num_paths[i] = 0;
            }
        }
    }

    return num_paths[major_order - 1];
}

// Input
//     snakes     : the pointer which points to the Position of snakes
//     num_snakes : the number of snakes
//     rows       : the number of rows
//     columns    : the number of columns
// Output
//     the number of different paths from top-left-most cell to the bottom-right-most cell of the grid
// Synopsis
//     traverse the matrix to calculate the number of paths according to the min(rows, columns)
int countAllPaths(Position* snakes, int num_snakes, int rows, int columns) {
    assert(isSnake(snakes, num_snakes, 0, 0) == FLATLAND);
    assert(isSnake(snakes, num_snakes, rows - 1, columns - 1) == FLATLAND);
    assert(((snakes != nullptr) && (num_snakes != 0)) || ((snakes == nullptr) && (num_snakes == 0)));

    int major_order = rows;
    int minor_order = columns;

    if(rows > columns) { // column major default, for row major case, transpose the matrix to avoid duplicate code
        for(int i = 0; i < num_snakes; i++) {
            Position tmp;
            tmp.swap(snakes[i]);
            snakes[i].first = tmp.second;
            snakes[i].second = tmp.first;
        }
        major_order = columns;
        minor_order = rows;
    }

    return countAllPathsByStride(snakes, num_snakes, major_order, minor_order);
}

// This case will cover the 3*3 matrix with 3 snake
void testZeroPath1() {
    constexpr int K = 3;
    Position snakes[K] = { Position(0, 2), Position(1, 1), Position(2, 0) };
    constexpr int rows = 3;
    constexpr int columns = 3;
    constexpr int result = 0;

    assert(countAllPaths(snakes, K, rows, columns) == result);
}

// This case will cover the 3*3 matrix with 2 snake
void testZeroPath2() {
    constexpr int K = 2;
    Position snakes[K] = { Position(0, 1), Position(1, 0) };
    constexpr int rows = 3;
    constexpr int columns = 3;
    constexpr int result = 0;

    assert(countAllPaths(snakes, K, rows, columns) == result);
}

// This case will cover the 3*3 matrix with 2 snake
void testZeroPath3() {
    constexpr int K = 2;
    Position snakes[K] = { Position(1, 2), Position(2, 1) };
    constexpr int rows = 3;
    constexpr int columns = 3;
    constexpr int result = 0;

    assert(countAllPaths(snakes, K, rows, columns) == result);
}

// This case will cover the 3*3 matrix with 2 snake
void testOnePath() {
    constexpr int K = 2;
    Position snakes[K] = { Position(2, 0), Position(1, 1) };
    constexpr int rows = 3;
    constexpr int columns = 3;
    constexpr int result = 1;

    assert(countAllPaths(snakes, K, rows, columns) == result);
}

// This case will cover the 3*3 matrix without snake
void testFullPath() {
    constexpr int K = 0;
    constexpr int rows = 3;
    constexpr int columns = 3;
    constexpr int result = 6;

    assert(countAllPaths(nullptr, K, rows, columns) == result);
}

// This case will cover the 2*3 matrix with 1 snake
void testNumPathOfRectMatrix1() {
    constexpr int K = 1;
    Position snakes[K] = { Position(1,1) };
    constexpr int rows = 2;
    constexpr int columns = 3;
    constexpr int result = 1;

    assert(countAllPaths(snakes, K, rows, columns) == result);
}

// This case will cover the 3*2 matrix with 1 snake
void testNumPathOfRectMatrix2() {
    constexpr int K = 1;
    Position snakes[K] = { Position(1,1) };
    constexpr int rows = 3;
    constexpr int columns = 2;
    constexpr int result = 1;

    assert(countAllPaths(snakes, K, rows, columns) == result);
}

int main() {
    testZeroPath1();
    testZeroPath2();
    testZeroPath3();
    testOnePath();
    testFullPath();
    testNumPathOfRectMatrix1();
    testNumPathOfRectMatrix2();
    return 0;
}
