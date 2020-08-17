
CC = gcc
OBJS = main.o Parser.o CommandList.o AuxList.o AuxParser.o LP.o BackTracking.o Board.o AuxBoard.o 
EXEC = sudoku-console
COMP_FLAG = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@
main.o: main.c Parser.h CommandList.h AuxList.h AuxParser.h LP.h Board.h AuxBoard.h 
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Parser.o: Parser.c CommandList.h AuxList.h AuxParser.h LP.h Board.h AuxBoard.h 
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
CommandList.o: CommandList.c AuxList.h AuxParser.h LP.h Board.h AuxBoard.h 
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
AuxList.o: AuxList.c  AuxParser.h Board.h AuxBoard.h 
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
AuxParser.o: AuxParser.c  Board.h AuxBoard.h 
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
LP.o: LP.c LP.h Board.h AuxBoard.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
BackTracking.o:  BackTracking.c BackTracking.h Board.c Board.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Board.o: Board.c Board.h AuxBoard.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
AuxBoard.o: AuxBoard.c AuxBoard.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c


clean:
	rm -f $(OBJS) $(EXEC)