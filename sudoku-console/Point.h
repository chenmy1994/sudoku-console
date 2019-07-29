//
// Created by CMY on 29/07/2019.
//

#ifndef SUDOKU_CONSOLE_POINT_H
#define SUDOKU_CONSOLE_POINT_H
/* Point contains:
 * x and y
 * */
typedef struct {
    int x;
    int y;
    int prev;
    int curr;
}Point;
#endif //SUDOKU_CONSOLE_POINT_H
