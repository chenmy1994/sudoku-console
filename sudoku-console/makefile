CC = gcc
OBJS = main.o Parser.o GeneralFunc.o EditFunc.o SolveFunc.o Game.o Board.o LinkedList.o Point.o Stack.o GurobiFunc.o
  
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors -g -O0
COMP_FLAGS_GUR = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
	
main.o: main.c Parser.h SPBufferset.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
Parser.o: Parser.c Parser.h GeneralFunc.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
GeneralFunc.o: GeneralFunc.c GeneralFunc.h EditFunc.h SolveFunc.h Stack.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
EditFunc.o: EditFunc.c EditFunc.h Game.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
SolveFunc.o: SolveFunc.c SolveFunc.h Game.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
Game.o: Game.c Game.h Board.h LinkedList.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
Board.o: Board.c Board.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
LinkedList.o: LinkedList.c LinkedList.h Point.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
Point.o: Point.c Point.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
Stack.o: Stack.c Stack.h Point.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
GurobiFunc.o: GurobiFunc.c GurobiFunc.h Game.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c

clean:
	rm -f $(OBJS) $(EXEC)