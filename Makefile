CC = clang
CFLAGS = -Wall -Wextra -g
LDLIBS = -lncurses

TARGET = game

OBJS = dungeon.o run_game.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

dungeon.o: dungeon.c dungeon.h
	$(CC) $(CFLAGS) -c dungeon.c

game_run.o: game_run.c dungeon.h
	$(CC) $(CFLAGS) -c run_game.c

clean:
	rm -f $(OBJS) $(TARGET)