CC = gcc
OBJS = main.o
 
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors -g -O0
COMP_FLAGS_GUR = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
	
main.o: main.c Parser.h SPBufferset.h
	$(CC) $(COMP_FLAG) -c $*.c
Parser.o: Parser.c GeneralFunc.h
	$(CC) $(COMP_FLAG) -c $*.c
GeneralFunc.o: GeneralFunc.c EditFunc.h SolveFunc.h Stack.h
	$(CC) $(COMP_FLAG) -c $*.c
EditFunc.o: EditFunc.c Game.h
	$(CC) $(COMP_FLAG) -c $*.c
SolveFunc.o: SolveFunc.c Game.h
	$(CC) $(COMP_FLAG) -c $*.c
Game.o: Game.c Board.h LinkedList.h
	$(CC) $(COMP_FLAG) -c $*.c
Board.o: Board.c
	$(CC) $(COMP_FLAG) -c $*.c
LinkedList.o: LinkedList.c Point.h
	$(CC) $(COMP_FLAG) -c $*.c
Point.o: Point.c
	$(CC) $(COMP_FLAG) -c $*.c
Stack.o: Stack.c Point.h
	$(CC) $(COMP_FLAG) -c $*.c
GurobiFunc.o: GurobiFunc.c Game.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c

clean:
	rm -f $(OBJS) $(EXEC)