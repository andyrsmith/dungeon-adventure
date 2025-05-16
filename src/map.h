#ifndef MAP_H
#define MAP_H

#include "types.h"

void initMap(char map[], Player *player, Monster monsters[]);

int getIndex(int x, int y, int width);

#endif // MAP_H
