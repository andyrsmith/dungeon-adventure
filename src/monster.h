#ifndef MONSTER_H
#define MONSTER_H

#include "types.h"

void findMonster(int x, int y, Monster monsters[], char map[]);

void moveMonsters(Monster monsters[], char map[], Player *player);

#endif // MONSTER_H
