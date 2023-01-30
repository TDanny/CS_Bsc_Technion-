CC = gcc
OBJS = chessSystem.o chessGame.o player.o tournament.o chessSystemTestsExample.o
EXEC = chess
DEBUG_FLAG = -g -DNDEBUG
COMP_FLAGS = -std=c99 -Wall -pedantic-errors -Werror
RM = rm -f
LIB_MAP =  libmap.a

$(EXEC): $(OBJS) $(LIB_MAP)
	$(CC) -o $(EXEC) $(DEBUG_FLAG) $(COMP_FLAGS) $(OBJS) $(LIB_MAP)

chessSystemTestsExample.o : tests/chessSystemTestsExample.c test_utilities.h chessSystem.h map.h player.h tournament.h chessGame.h 
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAGS) tests/chessSystemTestsExample.c

chessSystem.o : chessSystem.c chessSystem.h map.h player.h tournament.h chessGame.h 
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAGS) ./chessSystem.c

chessGame.o : chessGame.c chessGame.h chessSystem.h map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAGS) $*.c 

player.o : player.c player.h chessSystem.h map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAGS) $*.c

tournament.o : tournament.c tournament.h chessSystem.h map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAGS) $*.c

clean:
	$(RM) $(OBJS) $(LIB_MAP) $(EXEC)

