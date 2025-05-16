#ifndef TYPES_H
#define TYPES_H

#define MAP_WIDTH 80
#define MAP_HEIGHT 60
#define NUM_ROOMS 20

enum Player_State {
    rest,
    moving
};

typedef struct {
    int x;
    int y;
    int width;
    int height;
} Room;

typedef struct {
    int x;
    int y;
    enum Player_State state;
    int health;
} Player;

typedef struct {
    int x;
    int y;
    char type;
    int health;
} Monster;

typedef struct {
    int x;
    int y;
} Move_Position;

#endif // TYPES_H
