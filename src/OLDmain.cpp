
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "player.h"

//------------------------------------------------------------
// Global Setup
//------------------------------------------------------------

// global constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 640;
const int MAP_DISPLAY_WIDTH = 800;
const int MAP_DISPLAY_HEIGHT = 608;
const int MAX_HUNGER = 4000;
const int MAP_WIDTH = 100;
const int MAP_HEIGHT = 100; 
const int FOV = 9;
const int TEXTURE_TILE_SIZE = 32;
const int MAX_MONSTERS = 100;
const int MAX_ITEMS = 100;


// player initializer
Player player('@', 3, 3, 100, 4000, 10, 0);
class Mob
{
    public:
        char name;
        int x;
        int y;
        int hp;
        int atk;
        int type;

        Mob(char name, int x, int y, int hp, int atk, int type)
        {
            this->name = name;
            this->x = x;
            this->y = y;
            this->hp = hp;
            this->atk = atk;
            this->type = type;
        } 
};

class Item
{
    public:
        char name;
        int x;
        int y;
        int type;

        Item(char name, int x, int y, int hp)
        {
            this->name = name;
            this->x = x;
            this->y = y;
            this->type = type;
        }
};

//TileSet variables
SDL_Texture* tilesetTexture;
SDL_Texture* vignette;
SDL_Texture* playerTexture;
SDL_Texture* mobTexture;
SDL_Texture* fontTexture;
TTF_Font* font;

// gloabl variables
bool gameRunning = true;
int moveNumber = 1;
char map[MAP_WIDTH][MAP_HEIGHT];
int mapMemory[MAP_WIDTH][MAP_HEIGHT];
int mapTileSize = 32;

//
    //create a mob
    Mob mob1('R', 5, 5, 10, 10, 0);
//

// function prototypes
void drawGame(SDL_Renderer* renderer, char map[MAP_WIDTH][MAP_HEIGHT]);
void updateMemory();
void drawStats(SDL_Renderer* renderer);
void inputHandle();
void handleAI();
void drawTile(SDL_Renderer* renderer, char tileType, int x, int y);

//------------------------------------------------------------
// Function Definitions
//------------------------------------------------------------

void handleAI()
{
    //randomly move mob 1 tile or do not move
    int random = rand() % 4;
    switch (random)
    {
        case 0:
            if (map[mob1.x][mob1.y - 1] != '#')
            {
                mob1.y--;
            }
            break;
        case 1:
            if (map[mob1.x][mob1.y + 1] != '#')
            {
                mob1.y++;
            }
            break;
        case 2:
            if (map[mob1.x - 1][mob1.y] != '#')
            {
                mob1.x--;
            }
            break;
        case 3:
            if (map[mob1.x + 1][mob1.y] != '#')
            {
                mob1.x++;
            }
            break;
    }
    
}

void drawTile(SDL_Renderer* renderer, char tileType, int x, int y)
{
    // draw the tile
    SDL_Rect srcRect;
    SDL_Rect destRect;
    // set the source rectangle //TODO make more modular
    switch (tileType) 
    {
        case '#':
            srcRect = {0, 0, TEXTURE_TILE_SIZE, TEXTURE_TILE_SIZE};
            break;
        case '.':
            srcRect = {TEXTURE_TILE_SIZE, 0, TEXTURE_TILE_SIZE, TEXTURE_TILE_SIZE};
            break;
        case 'X':
            srcRect = {TEXTURE_TILE_SIZE * 2, 0, TEXTURE_TILE_SIZE, TEXTURE_TILE_SIZE};
            break;
        case ',':
            srcRect = {TEXTURE_TILE_SIZE * 3, 0, TEXTURE_TILE_SIZE, TEXTURE_TILE_SIZE};
            break;
    }
    // set the destination rectangle
    destRect = {x, y, mapTileSize, mapTileSize};
    // draw the tile
    SDL_RenderCopy(renderer, tilesetTexture, &srcRect, &destRect);
}

void drawGame(SDL_Renderer* renderer, char map[MAP_WIDTH][MAP_HEIGHT])
{
    // clear the screen
    SDL_SetRenderDrawColor(renderer, 10, 0, 0, 255);
    SDL_RenderClear(renderer);
    // draw the map
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            // calculate the screen coordinates of the tile
            int screenX = x * mapTileSize - player.x * mapTileSize + MAP_DISPLAY_WIDTH / 2;
            int screenY = y * mapTileSize - player.y * mapTileSize + MAP_DISPLAY_HEIGHT / 2;

            // use raycasting to determine if a '#' wass tile obscures tiles behind it
            bool visible = true;
            for (int i = 0; i < FOV; i++)
            {
                int rayX = player.x + i * (x - player.x) / FOV;
                int rayY = player.y + i * (y - player.y) / FOV;
                if (map[rayX][rayY] == '#')
                {
                    visible = false;
                    break;
                }
            }

        // calculate the distance between the tile and the player
        int distance = sqrt(pow(player.x - x, 2) + pow(player.y - y, 2));

        // check if the tile is within the player's field of view
        if (distance <= FOV && visible) 
        {
            // update the map memory
            mapMemory[x][y] = 1;
            // tile is within field of view, draw normally
            drawTile(renderer, map[x][y], screenX, screenY);
            //if tile is the same tile as mob1, draw mob1
            if (x == mob1.x && y == mob1.y)
            {
                // draw the mob with a bright pink tint
                SDL_SetTextureColorMod(mobTexture, 0, 0, 255);
                SDL_Rect srcRect = {0, 0, TEXTURE_TILE_SIZE, TEXTURE_TILE_SIZE};
                SDL_Rect destRect = {screenX, screenY, mapTileSize, mapTileSize};
                SDL_RenderCopy(renderer, mobTexture, &srcRect, &destRect);
                SDL_SetTextureColorMod(mobTexture, 255, 255, 255);

            }
        }
    
    else if (!visible && mapMemory[x][y] == 1 || distance > FOV && mapMemory[x][y] == 1)
    {
        // memory tiles are drawn with a little darker
        SDL_SetTextureAlphaMod(tilesetTexture, 255 * 0.2);
        drawTile(renderer, map[x][y], screenX, screenY);
        SDL_SetTextureAlphaMod(tilesetTexture, 255);
    }
}
    }
    // draw the player
    SDL_Rect srcRect = {0, 0, TEXTURE_TILE_SIZE, TEXTURE_TILE_SIZE};
    SDL_Rect destRect = {MAP_DISPLAY_WIDTH / 2, MAP_DISPLAY_HEIGHT / 2, mapTileSize, mapTileSize};
    SDL_RenderCopy(renderer, playerTexture, &srcRect, &destRect);

    // draw the stats
    drawStats(renderer);
    // update the screen
    SDL_RenderPresent(renderer);
}
void drawStats(SDL_Renderer* renderer)
{
    

}

void inputHandle() 
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                gameRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        gameRunning = false;
                        break;
                    case SDLK_UP:
                        if (map[player.x][player.y - 1] != '#')
                        {
                            player.y--;
                            moveNumber++;
                        }
                        handleAI();
                        break;
                    case SDLK_DOWN:
                        if (map[player.x][player.y + 1] != '#')
                        {
                            player.y++;
                            moveNumber++;
                        }
                        handleAI();
                        break;
                    case SDLK_LEFT:
                        if (map[player.x - 1][player.y] != '#')
                        {
                            player.x--;
                            moveNumber++;
                        }
                        handleAI();
                        break;
                    case SDLK_RIGHT:
                        if (map[player.x + 1][player.y] != '#')
                        {
                            player.x++;
                            moveNumber++;
                        }
                        handleAI();
                        break;
                    case SDLK_SPACE:
                        // zoom out
                        if (TEXTURE_TILE_SIZE > 1)
                        {
                            if (mapTileSize > 1)
                            {
                                mapTileSize = mapTileSize / 2;
                            }
                        }
                        break;
                    case SDLK_LSHIFT:
                        // zoom in
                        if (mapTileSize < 32)
                        {                    
                            mapTileSize = mapTileSize * 2;
                        }
                        break;
                }
                break;
        }
    }
}



//------------------------------------------------------------
// Main Function
//------------------------------------------------------------


int WinMain( int argc, char *argv[] )
{

    // seed the random number generator
    srand(time(NULL));
    // initialize SDL
    SDL_Init( SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow( "Roguelike", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );
    SDL_Renderer *renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
    TTF_Init();
    TTF_Font * font = TTF_OpenFont("assets/font.ttf", 12);;    
    SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
    SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );

    SDL_Surface* tilesetSurface = IMG_Load("assets/tileset.png");
    // create texture from the surface
    tilesetTexture = SDL_CreateTextureFromSurface(renderer, tilesetSurface);
    // load the player image to a surface
    SDL_Surface* playerSurface = IMG_Load("assets/player.png");

    // create player texture from the surface
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);

    // load the mob image to a surface
    SDL_Surface* mobSurface = IMG_Load("assets/mob.png");

    // create mob texture from the surface
    mobTexture = SDL_CreateTextureFromSurface(renderer, mobSurface);

    // load the map from a text file
    std::ifstream mapFile("map.txt");
    if (mapFile.is_open())
    {
        for (int y = 0; y < MAP_HEIGHT; y++)
        {
            for (int x = 0; x < MAP_WIDTH; x++)
            {
                mapFile >> map[x][y];
            }
        }
    }
    mapFile.close();

    //main game loop
    while (gameRunning)
    {
        // handle input
        inputHandle();

        // draw the game
        drawGame(renderer, map);
        // delay to get 60 fps
        SDL_Delay(1000 / 60);
    }


    // clean up
    SDL_DestroyTexture(tilesetTexture);
    SDL_FreeSurface(tilesetSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
