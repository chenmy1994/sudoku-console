//
// Created by CMY on 29/07/2019.
//

#ifndef SUDOKU_CONSOLE_BOARD_H
#define SUDOKU_CONSOLE_BOARD_H

/* Cell contains:
 * appendix = '.' if cell is fixed, '*' if cell is an erroneous, ' ' otherwise
 * val = the current game board
*/
typedef struct{
    char appendix;
    int val;
} Cell;

/* Block contains:
 * Two dimension array of Cells.
 * Will be set afterwards to be m*n.
 */
typedef struct {
    Cell ** block;
}Block;

/* Board contains:
 * Two dimension array of Blocks.
 * Will be set afterwards to be n*m.
 * markError - 1 if erroneous is displayed, 0 otherwise.
 */
typedef struct {
    Block ** board;
    int markError;
}Board;

/*Free all board memory*/
void destroyBoard(Board* board);


#endif //SUDOKU_CONSOLE_BOARD_H
