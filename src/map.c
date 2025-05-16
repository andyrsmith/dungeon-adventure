#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "types.h"
#include "map.h"

void initMap(char map[], Player *player, Monster monsters[]) {
    for(int i = 0; i<(80*60); i++) {
        map[i] = '#';
    }

    int num_rooms = 0;
    //creating rooms
    Room rooms[20];
    srand(time(NULL));
    for(int x=0; x<20; x++) {
 
        bool overlap = false;
        Room room = {
            rand() % MAP_WIDTH + 1,
            rand() % MAP_HEIGHT + 1,
            rand() % 8 + 2,
            rand() % 8 + 2
        };
        
        if(x>0) {
            for(int y=0; y<x; y++) {
                if(!(room.x + room.width < rooms[y].x || 
                         room.x > rooms[y].x + rooms[y].width || 
                         room.y + room.height < rooms[y].y || 
                         room.y > rooms[y].y + rooms[y].height))  
                     {
                    overlap = true;
                    x--;
                    break;
                }
            }
            if(!overlap) {
                num_rooms++;
                rooms[x] = room;
            }
        } else {
            num_rooms++;
            rooms[x] = room;
        }
    };
    int player_x = (rooms[0].width/2) + rooms[0].x;
    int player_y = (rooms[0].height/2) + rooms[0].y;
    player->x = player_x;
    player->y = player_y;
    for(int r=0; r<num_rooms; r++) {
       for(int r2=rooms[r].x; r2<(rooms[r].x + rooms[r].width); r2++) {
            for(int y=rooms[r].y; y<(rooms[r].y + rooms[r].height); y++) {
                //move the room out of bounds
                if((y*MAP_WIDTH)+r2 < (80*60)) {
                    map[(y*MAP_WIDTH)+r2] = '.';
                }
            }
        }
    } 

    // make corridors between rooms
    for(int c = 1; c < num_rooms - 1; c++) {
        int x1 = rooms[c].x + (rooms[c].width / 2);
        int y1 = rooms[c].y + (rooms[c].height / 2);
        int x2 = rooms[c + 1].x + (rooms[c + 1].width / 2);
        int y2 = rooms[c + 1].y + (rooms[c + 1].height / 2);

        // Draw horizontal corridor
        if(x1 > x2) {
            for(int x = x2; x <= x1; x++) {
                if((y1*MAP_WIDTH) + x) {
                    map[(y1 * MAP_WIDTH) + x] = '.';
                }
            }
        } else {
            for(int x = x1; x <= x2; x++) {
                if((y1*MAP_WIDTH) + x) {
                    map[(y1 * MAP_WIDTH) + x] = '.';
                }
            }
        }

        // Draw vertical corridor
        if(y1 > y2) {
            for(int y = y2; y <= y1; y++) {
                if((y * MAP_WIDTH) + x2) {
                    map[(y * MAP_WIDTH) + x2] = '.';
                }
            }
        } else {
            for(int y = y1; y <= y2; y++) {
                if((y*MAP_WIDTH) + x2) {
                    map[(y * MAP_WIDTH) + x2] = '.';
                }
            }
        }
    }

    for(int m = 1; m < num_rooms; m++) {
        int monster_x = (rooms[m].width/2) + rooms[m].x;
        int monster_y = (rooms[m].height/2) + rooms[m].y;
        if(getIndex(monster_x, monster_y, MAP_WIDTH) < (80*60)) {
            monsters[m].x = monster_x;
            monsters[m].y = monster_y;
            map[getIndex(monster_x, monster_y, MAP_WIDTH)] = 'M';
            monsters[m].type = 'M';
            monsters[m].health = 1;
        }
    }
}

int getIndex(int x, int y, int width) {
    return (y * width) + x;
}
