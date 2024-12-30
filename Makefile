CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = minesweeper

SRCS = main.c gameWithFile.c initGame.c instruction.c printBoard.c score.c
OBJS = $(SRCS:.c=.o)
HEADERS = cell.h gameWithFile.h initGame.h instruction.h printBoard.h score.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	./$(TARGET) -f game.txt
	./$(TARGET) -f gameLose.txt
	./$(TARGET) -f game-error.txt
	./$(TARGET) -f game-error2.txt
	./$(TARGET) -f game-error3.txt
	./$(TARGET) -f game-error4.txt

.PHONY: all clean run test
