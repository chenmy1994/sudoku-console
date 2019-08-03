/*
 * Point.h
 *
 *  Created on: Aug 1, 2019
 *      Author: Noa
 */

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

/*set x value of a given point p*/
void setX(Point* p, int x);

/*set y value of a given point p*/
void setY(Point* p, int y);

#endif /*SUDOKU_CONSOLE_POINT_H*/
