#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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
} Player;

typedef struct {
    int x;
    int y;
    char type;
} Monster;

typedef struct {
    int x;
    int y;
} Move_Position;

void initMap(char map[], Player *player, Monster monsters[]);

int getIndex(int x, int y, int width);

int main() {
    char map[MAP_WIDTH * MAP_HEIGHT];

    Player player = {0, 0, rest};
    Monster monsters[19];

    Move_Position move = {0,0};

    initMap(map, &player, monsters);

    InitWindow(800, 600, "Rogue Dungeon");

    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        //get player movement
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            Vector2 mouse_position = GetMousePosition();
            move.x = mouse_position.x/10;
            move.y = mouse_position.y/10;

            player.state = moving;

        };
        // move player one frame at a time
        if(player.state == moving) {
            if(move.x < player.x && getIndex(player.x-1, player.y, MAP_WIDTH) >= 0 && map[getIndex(player.x-1, player.y, MAP_WIDTH)] != '#') {
                player.x--;
            } else if(move.x > player.x && getIndex(player.x+1, player.y, MAP_WIDTH) < MAP_WIDTH * MAP_HEIGHT && map[getIndex(player.x+1, player.y, MAP_WIDTH)] != '#') {
                player.x++;
            } else if(move.y < player.y && getIndex(player.x, player.y-1, MAP_WIDTH) >= 0 && map[getIndex(player.x, player.y-1, MAP_WIDTH)] != '#') {
                player.y--;
            } else if(move.y > player.y && getIndex(player.x, player.y+1, MAP_WIDTH) < MAP_WIDTH * MAP_HEIGHT && map[getIndex(player.x, player.y+1, MAP_WIDTH)] != '#') {
                player.y++;
            }

            if(move.x == player.x && move.y == player.y) {
                player.state = rest;
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);
            for(int i=0; i<(MAP_WIDTH*MAP_HEIGHT); i++) {
                if(map[i] == '#') 
                {
                    DrawRectangle((i%MAP_WIDTH) * 10, (i/MAP_WIDTH) * 10, 10, 10, GRAY);
                } else if(map[i] == '.') {
                    DrawRectangle((i%MAP_WIDTH) * 10, (i/MAP_WIDTH) * 10, 10, 10, BLACK);
                }
            }
            DrawRectangle((player.x)*10, (player.y)*10, 10, 10, RED);
            for(int r=0; r<NUM_ROOMS; r++) {
                if(monsters[r].type == 'M') {
                    DrawRectangle((monsters[r].x)*10, (monsters[r].y)*10, 10, 10, BLUE);
                }
            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

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
    printf("hello\n");
    printf("Number of rooms %d \n", num_rooms);
    int player_x = (rooms[0].width/2) + rooms[0].x;
    int player_y = (rooms[0].height/2) + rooms[0].y;
    player->x = player_x;
    player->y = player_y;
    for(int r=0; r<num_rooms; r++) {
        printf("Room Number: %d\n", r);
       for(int r2=rooms[r].x; r2<(rooms[r].x + rooms[r].width); r2++) {
            for(int y=rooms[r].y; y<(rooms[r].y + rooms[r].height); y++) {
                //move the room out of bounds
                if((y*MAP_WIDTH)+r2 <= (80*60)) {
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

    for(int m = 0; m <= num_rooms; m++) {
        int monster_x = (rooms[m+1].width/2) + rooms[m+1].x;
        int monster_y = (rooms[m+1].height/2) + rooms[m+1].y;
        monsters[m].x = monster_x;
        monsters[m].y = monster_y;
        monsters[m].type = 'M';
    }
}

int getIndex(int x, int y, int width) {
    return (y * width) + x;
}
