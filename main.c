#include "raylib.h"
#include "src/types.h"
#include "src/map.h"
#include "src/monster.h"
#include <stdio.h>

typedef struct {
    Texture2D tileset;
    int tileWidth;
    int tileHeight;
    int tilesPerRow;
    int tileSpace;
} Tilemap;

#define TILE_SIZE 16

Rectangle GetTileRect(Tilemap tilemap, int tileID) {
    int row = tileID / tilemap.tilesPerRow;
    int col = tileID % tilemap.tilesPerRow;

    return (Rectangle) {
        (col * tilemap.tileWidth) + col,
        (row * tilemap.tileHeight) + row,
        tilemap.tileWidth,
        tilemap.tileHeight
    };
}

int main() {
    char map[MAP_WIDTH * MAP_HEIGHT];

    Player player = {0, 0, rest, 20};
    Monster monsters[19];

    Move_Position move = {0,0};

    initMap(map, &player, monsters);

    Camera2D camera = { 0 };
    camera.target = (Vector2){ player.x * TILE_SIZE, player.y * TILE_SIZE };
    camera.offset = (Vector2){ 800/2.0f, 600/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 2.0f;

    InitWindow(800, 600, "Rogue Dungeon");

    Tilemap dungeonTileMap;
    dungeonTileMap.tileset = LoadTexture("resources/roguelikeDungeon_transparent.png");
    printf("Texture loaded");
    dungeonTileMap.tileWidth = 16;
    dungeonTileMap.tileHeight = 16;
    dungeonTileMap.tilesPerRow = 29;
    dungeonTileMap.tileSpace = 1;

    Tilemap characterTileMap;
    characterTileMap.tileset = LoadTexture("resources/roguelikeChar_transparent.png");
    printf("Texture loaded");
    characterTileMap.tileWidth = 16;
    characterTileMap.tileHeight = 16;
    characterTileMap.tilesPerRow = 56;
    characterTileMap.tileSpace = 1;


    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        //get player movement
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            Vector2 mouse_position = GetMousePosition();
            move.x = (mouse_position.x/TILE_SIZE) - ((800/2)/TILE_SIZE);
            move.y = (mouse_position.y/TILE_SIZE) - ((600/2)/TILE_SIZE);
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
                camera.target = (Vector2){ player.x * TILE_SIZE, player.y * TILE_SIZE};
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
                    Rectangle sourceRect = GetTileRect(dungeonTileMap, 17);
                    Vector2 position = {(i%MAP_WIDTH) * TILE_SIZE, (i/MAP_WIDTH) * TILE_SIZE};
                    DrawTextureRec(dungeonTileMap.tileset, sourceRect, position, WHITE);
                    //DrawRectangle((i%MAP_WIDTH) * TILE_SIZE, (i/MAP_WIDTH) * TILE_SIZE, TILE_SIZE, TILE_SIZE, GRAY);
                } else if(map[i] == '.') {
                    DrawRectangle((i%MAP_WIDTH) * TILE_SIZE, (i/MAP_WIDTH) * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLACK);
                } else if(map[i] == 'M') {
                    //DrawRectangle((i%MAP_WIDTH) * TILE_SIZE, (i/MAP_WIDTH) * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLUE);
                    Rectangle monsterRect = GetTileRect(characterTileMap, 168);
                    Vector2 monsterPosition = {(i%MAP_WIDTH) * TILE_SIZE, (i/MAP_WIDTH) * TILE_SIZE};
                    DrawTextureRec(characterTileMap.tileset, monsterRect, monsterPosition, WHITE);

                }
            }
            //DrawRectangle((player.x)*TILE_SIZE, (player.y)*TILE_SIZE, TILE_SIZE, TILE_SIZE, RED);
            Rectangle playerRect = GetTileRect(characterTileMap, 336);
            Vector2 playerPosition = {player.x* TILE_SIZE, player.y* TILE_SIZE};
            DrawTextureRec(characterTileMap.tileset, playerRect, playerPosition, WHITE);


            EndMode2D();
        EndDrawing();
    }
    UnloadTexture(dungeonTileMap.tileset);
    CloseWindow();

    return 0;
}

