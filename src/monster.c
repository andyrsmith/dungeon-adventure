#include <stdlib.h>
#include "types.h"
#include "raylib.h"
#include "map.h"

void findMonster(int x, int y, Monster monsters[], char map[]) {
    for(int i = 0; i < 19; i++) {
        if(monsters[i].x == x && monsters[i].y == y) {
            monsters[i].health--;
            if(monsters[i].health <= 0) {
                monsters[i].x = -1;
                monsters[i].y = -1;
                map[getIndex(x, y, MAP_WIDTH)] = '.';
            }
        }
    }
}

void moveMonsters(Monster monsters[], char map[], Player *player) {
    for(int i = 0; i < 19; i++) {
        if(monsters[i].x != -1 && monsters[i].y != -1) {
            int move_x = rand() % 3 - 1;
            int move_y = rand() % 3 - 1;
            if(map[getIndex(monsters[i].x + move_x, monsters[i].y + move_y, MAP_WIDTH)] == '.') {
                map[getIndex(monsters[i].x, monsters[i].y, MAP_WIDTH)] = '.';
                monsters[i].x += move_x;
                monsters[i].y += move_y;
                map[getIndex(monsters[i].x, monsters[i].y, MAP_WIDTH)] = 'M';
            }

            if(monsters[i].x == player->x && monsters[i].y == player->y) { 
                player->health--;
                if(player->health <= 0) {
                    CloseWindow();
                }
            }
        }
    }
}

