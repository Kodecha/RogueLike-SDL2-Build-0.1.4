#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "include/Player.h"

using namespace std;

// Global variables
const int WINDOW_WIDTH = 896;
const int WINDOW_HEIGHT = 640;
const int MAP_DISPLAY_WIDTH = 752;
const int MAP_DISPLAY_HEIGHT = 672;
const int MAP_WIDTH = 100;
const int MAP_HEIGHT = 100; 
const int STATS_WIDTH = 128;
const int STATS_HEIGHT = 192;
const int STATS_X = WINDOW_WIDTH - STATS_WIDTH;
const int STATS_Y = 0;
const int FOV = 9;
const int TEXTURE_TILE_SIZE = 32;
const int MAX_MONSTERS = 100;
const int MAX_ITEMS = 100;

int currentFrame = 0;
int currentMove = 0;

// Global variables for SDL
SDL_Texture* tilesetTexture;
SDL_Texture* vignette;
SDL_Texture* playerTexture;
SDL_Texture* mobTexture;
SDL_Texture* fontTexture;
TTF_Font* font;

SDL_Window* window;
SDL_Renderer* renderer;

// Stand in Vars

    // Define stat values
    int hp = 10;
    int maxHp = 10;
    int mana = 6;
    int maxMana = 15;
    int level = 3;
    int maxExp = 100;
    int strength = 5;
    int armor = 10;
    int dexterity = 5;
    int intelligence = 12;
    int wisdom = 2;
    int physicalAttack = 10;

// Global variables for game
bool gameRunning = true;
char map[MAP_WIDTH][MAP_HEIGHT];
int mapMemory[MAP_WIDTH][MAP_HEIGHT];
int mapTileSize = 32; // size of each tile in pixels, effects "zoom"

// initialize a player
Player player("NoName", 3, 3);

void drawGame(SDL_Renderer* renderer, char map[MAP_WIDTH][MAP_HEIGHT]);
void updateMemory();
void drawStats(SDL_Renderer* renderer, TTF_Font* font);
void inputHandle();
void update();
void drawTile(SDL_Renderer* renderer, char tileType, int x, int y);

//------------------------------------------------------------
// Function Definitions
//------------------------------------------------------------

void update()
{
    currentMove++;
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
        int screenX = (x - player.getX() + FOV / 2) * mapTileSize + (MAP_DISPLAY_WIDTH - FOV * mapTileSize) / 2;
        int screenY = (y - player.getY() + FOV / 2) * mapTileSize + (MAP_DISPLAY_HEIGHT - FOV * mapTileSize) / 2 + (WINDOW_HEIGHT - MAP_DISPLAY_HEIGHT) / 2;

            // use raycasting to determine if a '#' wass tile obscures tiles behind it
            bool visible = true;
            for (int i = 0; i < FOV; i++)
            {
                int rayX = player.getX() + i * (x - player.getX()) / FOV;
                int rayY = player.getY() + i * (y - player.getY()) / FOV;
                if (map[rayX][rayY] == '#')
                {
                    visible = false;
                    break;
                }
            }

        // calculate the distance between the tile and the player
        int distance = sqrt(pow(player.getX() - x, 2) + pow(player.getY() - y, 2));

        // check if the tile is within the player's field of view
        if (distance <= FOV && visible) 
        {
            // update the map memory
            mapMemory[x][y] = 1;
            // tile is within field of view, draw normally
            drawTile(renderer, map[x][y], screenX, screenY);
        }
    // check if the tile is not within the player's field of view and is in the map memory or is not in the map memory
    else if ((!visible && mapMemory[x][y] == 1) || (distance > FOV && mapMemory[x][y] == 1))
    {
        // memory tiles are drawn with a little darker
        SDL_SetTextureAlphaMod(tilesetTexture, 255 * 0.15);
        drawTile(renderer, map[x][y], screenX, screenY);
        SDL_SetTextureAlphaMod(tilesetTexture, 255);
    }
}
    }
    // draw the player
    SDL_Rect srcRect = {0, 0, TEXTURE_TILE_SIZE, TEXTURE_TILE_SIZE};
    SDL_Rect destRect = {(MAP_DISPLAY_WIDTH - mapTileSize) / 2, (MAP_DISPLAY_HEIGHT - mapTileSize) / 2 + (WINDOW_HEIGHT - MAP_DISPLAY_HEIGHT) / 2, mapTileSize, mapTileSize};
    SDL_RenderCopy(renderer, playerTexture, &srcRect, &destRect);

    // draw the stats
    drawStats(renderer, font);

}

void drawStats(SDL_Renderer* renderer, TTF_Font* font)
{
    // Draw the background box
    SDL_Rect backgroundRect = {STATS_X, STATS_Y, STATS_WIDTH, STATS_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255); // Dark navy blue color
    SDL_RenderFillRect(renderer, &backgroundRect);

    // Set the text color
    SDL_Color textColor = {255, 255, 255, 255}; // White color

    // Set the bar colors
    SDL_Color healthBarColor = {255, 0, 0, 255};    // Red color
    SDL_Color manaBarColor = {0, 0, 255, 255};      // Blue color
    SDL_Color expBarColor = {255, 255, 0, 255};     // Yellow color

    // Draw the stats text
    string statsText = "HP: " + to_string(hp) + "/" + to_string(maxHp);
    SDL_Surface* hpTextSurface = TTF_RenderText_Solid(font, statsText.c_str(), textColor);
    SDL_Texture* hpTextTexture = SDL_CreateTextureFromSurface(renderer, hpTextSurface);
    SDL_Rect hpTextRect = {STATS_X + 10, STATS_Y + 10, STATS_WIDTH - 20, 20};
    SDL_QueryTexture(hpTextTexture, nullptr, nullptr, &hpTextRect.w, &hpTextRect.h);
    SDL_RenderCopy(renderer, hpTextTexture, nullptr, &hpTextRect);
    SDL_FreeSurface(hpTextSurface);
    SDL_DestroyTexture(hpTextTexture);

    // Draw the health bar
    float hpPercentage = static_cast<float>(hp) / maxHp;
    int hpBarWidth = static_cast<int>(hpPercentage * (STATS_WIDTH - 20));
    SDL_Rect hpBarRect = {STATS_X + 10, STATS_Y + 40, hpBarWidth, 10};
    SDL_SetRenderDrawColor(renderer, healthBarColor.r, healthBarColor.g, healthBarColor.b, healthBarColor.a);
    SDL_RenderFillRect(renderer, &hpBarRect);

    // Draw the mana text
    statsText = "MANA: " + to_string(mana) + "/" + to_string(maxMana);
    SDL_Surface* manaTextSurface = TTF_RenderText_Solid(font, statsText.c_str(), textColor);
    SDL_Texture* manaTextTexture = SDL_CreateTextureFromSurface(renderer, manaTextSurface);
    SDL_Rect manaTextRect = {STATS_X + 10, STATS_Y + 60, STATS_WIDTH - 20, 20};
    SDL_QueryTexture(manaTextTexture, nullptr, nullptr, &manaTextRect.w, &manaTextRect.h);
    SDL_RenderCopy(renderer, manaTextTexture, nullptr, &manaTextRect);
    SDL_FreeSurface(manaTextSurface);
    SDL_DestroyTexture(manaTextTexture);

    // Draw the mana bar
    float manaPercentage = static_cast<float>(mana) / maxMana;
    int manaBarWidth = static_cast<int>(manaPercentage * (STATS_WIDTH - 20));
    SDL_Rect manaBarRect = {STATS_X + 10, STATS_Y + 90, manaBarWidth, 10};
    SDL_SetRenderDrawColor(renderer, manaBarColor.r, manaBarColor.g, manaBarColor.b, manaBarColor.a);
    SDL_RenderFillRect(renderer, &manaBarRect);

    // Draw the level text
    statsText = "LVL: " + to_string(level);
    SDL_Surface* levelTextSurface = TTF_RenderText_Solid(font, statsText.c_str(), textColor);
    SDL_Texture* levelTextTexture = SDL_CreateTextureFromSurface(renderer, levelTextSurface);
    SDL_Rect levelTextRect = {STATS_X + 10, STATS_Y + 110, STATS_WIDTH - 20, 20};
    SDL_QueryTexture(levelTextTexture, nullptr, nullptr, &levelTextRect.w, &levelTextRect.h);
    SDL_RenderCopy(renderer, levelTextTexture, nullptr, &levelTextRect);
    SDL_FreeSurface(levelTextSurface);
    SDL_DestroyTexture(levelTextTexture);

    // Draw the experience text
    statsText = "EXP: " + to_string(11) + "/" + to_string(maxExp);
    SDL_Surface* expTextSurface = TTF_RenderText_Solid(font, statsText.c_str(), textColor);
    SDL_Texture* expTextTexture = SDL_CreateTextureFromSurface(renderer, expTextSurface);
    SDL_Rect expTextRect = {STATS_X + 10, STATS_Y + 130, STATS_WIDTH - 20, 20};
    SDL_QueryTexture(expTextTexture, nullptr, nullptr, &expTextRect.w, &expTextRect.h);
    SDL_RenderCopy(renderer, expTextTexture, nullptr, &expTextRect);
    SDL_FreeSurface(expTextSurface);
    SDL_DestroyTexture(expTextTexture);

    // Draw the experience bar
    float expPercentage = static_cast<float>(11) / maxExp;
    int expBarWidth = static_cast<int>(expPercentage * (STATS_WIDTH - 20));
    SDL_Rect expBarRect = {STATS_X + 10, STATS_Y + 160, expBarWidth, 10};
    SDL_SetRenderDrawColor(renderer, expBarColor.r, expBarColor.g, expBarColor.b, expBarColor.a);
    SDL_RenderFillRect(renderer, &expBarRect);
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
                        if (map[player.getX()][player.getY() - 1] != '#')
                        {
                            player.setY(player.getY() - 1);
                        }
                        update();
                        break;
                    case SDLK_DOWN:
                        if (map[player.getX()][player.getY() + 1] != '#')
                        {
                            player.setY(player.getY() + 1);
                        }
                        update();
                        break;
                    case SDLK_LEFT:
                        if (map[player.getX() - 1][player.getY()] != '#')
                        {
                            player.setX(player.getX() - 1);
                        }
                        update();
                        break;
                    case SDLK_RIGHT:
                        if (map[player.getX() + 1][player.getY()] != '#')
                        {
                            player.setX(player.getX() + 1);
                        }
                        update();
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
int WinMain(int argc, char* argv[]) 
{

    // seed the random number generator
    srand(time(NULL));
    // initialize SDL
    SDL_Init( SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow( "Roguelike", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );
    SDL_Renderer *renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

    // initialize SDL_ttf
    TTF_Init();    
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

    // Load font
    TTF_Font* font = TTF_OpenFont("assets/font.ttf", 18);
    
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
        // draw stats box
        drawStats(renderer, font);
        // update the screen
        SDL_RenderPresent(renderer);
        // delay to get 45 fps
        SDL_Delay(1000 / 45);
        currentFrame++;
    }

    // Clean up
    SDL_DestroyTexture(tilesetTexture);
    SDL_DestroyTexture(vignette);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(mobTexture);
    SDL_DestroyTexture(fontTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
