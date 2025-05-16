CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lraylib -lm -lpthread -ldl -lGL -lX11

TARGET = dungeon

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c src/map.c src/monster.c $(LDFLAGS)

clean:
	rm -f $(TARGET)
