#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include "src/types.h"
#include "src/map.h"


void findMonster(int x, int y, Monster monsters[], char map[]);

void moveMonsters(Monster monsters[], char map[], Player *player);

int main() {
    char map[MAP_WIDTH * MAP_HEIGHT];

    Player player = {0, 0, rest, 20};
    Monster monsters[19];

    Move_Position move = {0,0};

    initMap(map, &player, monsters);
    Camera2D camera = { 0 };
    camera.target = (Vector2){ player.x * 10, player.y * 10 };
    camera.offset = (Vector2){ 800/2.0f, 600/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    InitWindow(800, 600, "Rogue Dungeon");

    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        //get player movement
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            Vector2 mouse_position = GetMousePosition();
            move.x = (mouse_position.x/10) - 40;
            move.y = (mouse_position.y/10) - 30;
            move.x = move.x + player.x;
            move.y = move.y + player.y;

            player.state = moving;

        };
        // move player one frame at a time
        if(player.state == moving) {
            Move_Position next_move = {player.x, player.y};
            if(move.x < player.x && getIndex(player.x-1, player.y, MAP_WIDTH) >= 0) {
                if(map[getIndex(player.x-1, player.y, MAP_WIDTH)] == '#') {
                    move.x = next_move.x;
                } else {
                    next_move.x--;
                }
            } else if(move.x > player.x && getIndex(player.x+1, player.y, MAP_WIDTH) < MAP_WIDTH * MAP_HEIGHT) {
                if(map[getIndex(player.x+1, player.y, MAP_WIDTH)] == '#') {
                    move.x = next_move.x;
                } else {
                    next_move.x++;
                }
            } else if(move.y < player.y && getIndex(player.x, player.y-1, MAP_WIDTH) >= 0) {
                if(map[getIndex(player.x, player.y-1, MAP_WIDTH)] == '#') {
                    move.y = next_move.y;
                } else {
                    next_move.y--;
                }
            } else if(move.y > player.y && getIndex(player.x, player.y+1, MAP_WIDTH) < MAP_WIDTH * MAP_HEIGHT) {
                if(map[getIndex(player.x, player.y+1, MAP_WIDTH)] == '#') {
                    move.y = next_move.y;
                } else {
                    next_move.y++;
                }
            }

            if(map[getIndex(next_move.x, next_move.y, MAP_WIDTH)] == 'M') {
                findMonster(next_move.x, next_move.y, monsters, map);
                player.state = rest;
            } else {
                player.x = next_move.x;
                player.y = next_move.y;
                camera.target = (Vector2){ player.x * 10, player.y * 10};
                camera.offset = (Vector2){ 800/2.0f, 600/2.0f };
            }

            if(move.x == player.x && move.y == player.y) {
                moveMonsters(monsters, map, &player);
                player.state = rest;
            }
        }


        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode2D(camera);

            for(int i=0; i<(MAP_WIDTH*MAP_HEIGHT); i++) {
                if(map[i] == '#') 
                {
                    DrawRectangle((i%MAP_WIDTH) * 10, (i/MAP_WIDTH) * 10, 10, 10, GRAY);
                } else if(map[i] == '.') {
                    DrawRectangle((i%MAP_WIDTH) * 10, (i/MAP_WIDTH) * 10, 10, 10, BLACK);
                } else if(map[i] == 'M') {
                    DrawRectangle((i%MAP_WIDTH) * 10, (i/MAP_WIDTH) * 10, 10, 10, BLUE);
                }
            }
            DrawRectangle((player.x)*10, (player.y)*10, 10, 10, RED);

            EndMode2D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void findMonster(int x, int y, Monster monsters[], char map[]) {
    for(int i = 0; i < 19; i++) {
        if(monsters[i].x == x && monsters[i].y == y) {
            printf("Found monster at %d %d\n", x, y);
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
                printf("Monster found player at %d %d\n", monsters[i].x, monsters[i].y);
                player->health--;
                if(player->health <= 0) {
                    printf("Player is dead\n");
                    CloseWindow();
                }
            }
        }
    }
}

