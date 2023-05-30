#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Player.h"

using namespace std;

// Global variables
const int WINDOW_WIDTH = 896;
const int WINDOW_HEIGHT = 640;
const int MAP_DISPLAY_WIDTH = 752;
const int MAP_DISPLAY_HEIGHT = 672;
const int MAP_WIDTH = 100;
const int MAP_HEIGHT = 100; 
const int STATS_WIDTH = 128;
const int STATS_HEIGHT = 240;
const int STATS_X = WINDOW_WIDTH - STATS_WIDTH;
const int STATS_Y = 0;
const int FOV = 9;
const int TEXTURE_TILE_SIZE = 32;
const int MAX_MONSTERS = 100;
const int MAX_ITEMS = 100;
const int SEED = 1997;

int currentFrame = 0;
int currentMove = 0;
int animationFrame = 0;

// Global variables for SDL
SDL_Texture* tilesetTexture;
SDL_Texture* vignette;
SDL_Texture* playerTexture;
SDL_Texture* mobTexture;
SDL_Texture* fontTexture;
TTF_Font* font;

SDL_Window* window;
SDL_Renderer* renderer;

// Global variables for game
bool gameRunning = true;
char map[MAP_WIDTH][MAP_HEIGHT];
int mapMemory[MAP_WIDTH][MAP_HEIGHT];
int mapTileSize = 32; // size of each tile in pixels, effects "zoom"

//------------------------------------------------------------
// Function Definitions
//------------------------------------------------------------
void update()//TODO create and add to a game class
{
    currentMove++;
}

void updateAnimationFrame(int currentFrame) // TODO add to a draw class
{
    if (currentFrame % 20 == 0)
    {
        animationFrame++;
        if (animationFrame > 3)
        {
            animationFrame = 0;
        }
    }
}
void drawTile(SDL_Renderer* renderer, char tileType, int x, int y) // TODO add to a draw class
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

void drawGame(SDL_Renderer* renderer, char map[MAP_WIDTH][MAP_HEIGHT], Player player) //TODO add to draw class
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
        int screenX = (x - player.getStat("x") + FOV / 2) * mapTileSize + (MAP_DISPLAY_WIDTH - FOV * mapTileSize) / 2;
        int screenY = (y - player.getStat("y") + FOV / 2) * mapTileSize + (MAP_DISPLAY_HEIGHT - FOV * mapTileSize) / 2 + (WINDOW_HEIGHT - MAP_DISPLAY_HEIGHT) / 2;

            // checks if a wall "#" obscures the tiles behind it
            bool visible = true;
            for (int i = 0; i < FOV; i++)
            {
                int rayX = player.getStat("x") + (x - player.getStat("x")) * i / FOV;
                int rayY = player.getStat("y") + (y - player.getStat("y")) * i / FOV;
                if (map[rayX][rayY] == '#')
                {
                    visible = false;
                    break;
                }
            }
        // calculate the distance between the tile and the player
        int distance = sqrt(pow(player.getStat("x") - x, 2) + pow(player.getStat("y") - y, 2));

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
        SDL_SetTextureAlphaMod(tilesetTexture, 255 * 0.40);
        drawTile(renderer, map[x][y], screenX, screenY);
        SDL_SetTextureAlphaMod(tilesetTexture, 255);
    }
}
    }
    // draw the player
    SDL_Rect srcRect = {TEXTURE_TILE_SIZE * animationFrame, 0, TEXTURE_TILE_SIZE, TEXTURE_TILE_SIZE};
    SDL_Rect destRect = {(MAP_DISPLAY_WIDTH - mapTileSize) / 2, (MAP_DISPLAY_HEIGHT - mapTileSize) / 2 + (WINDOW_HEIGHT - MAP_DISPLAY_HEIGHT) / 2, mapTileSize, mapTileSize};
    SDL_RenderCopy(renderer, playerTexture, &srcRect, &destRect);
}

void drawStats(SDL_Renderer* renderer, TTF_Font* font, Player player) //TODO add to draw class
{
    // Draw the background box
    SDL_Rect backgroundRect = {STATS_X, STATS_Y, STATS_WIDTH, STATS_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255); // Dark navy blue color
    SDL_RenderFillRect(renderer, &backgroundRect);

    // Set the text color
    SDL_Color textColor = {255, 255, 255, 255}; // White color

    // Set the bar colors
    SDL_Color healthBarColor = {255, 0, 0, 255};    // Red color
    SDL_Color manaBarColor = {0, 0, 255, 255};      // Blue color
    SDL_Color expBarColor = {255, 255, 0, 255};     // Yellow color

    // Draw the stats text
    string statsText = "HP: " + to_string(player.getStat("hp")) + "/" + to_string(player.getStat("maxHp"));
    SDL_Surface* hpTextSurface = TTF_RenderText_Solid(font, statsText.c_str(), textColor); // this is inefficient, but it's fine for now
    SDL_Texture* hpTextTexture = SDL_CreateTextureFromSurface(renderer, hpTextSurface);
    SDL_Rect hpTextRect = {STATS_X + 10, STATS_Y + 20, STATS_WIDTH - 20, 20};
    SDL_QueryTexture(hpTextTexture, nullptr, nullptr, &hpTextRect.w, &hpTextRect.h);
    SDL_RenderCopy(renderer, hpTextTexture, nullptr, &hpTextRect);
    SDL_FreeSurface(hpTextSurface);
    SDL_DestroyTexture(hpTextTexture);

    // Draw the health bar
    float healthPercentage = static_cast<float>(player.getStat("hp")) / player.getStat("maxHp");
    int healthBarWidth = static_cast<int>((STATS_WIDTH - 20) * healthPercentage);
    int healthBarHeight = 5;
    SDL_Rect healthBarRect = {STATS_X + 10, STATS_Y + 50, healthBarWidth, healthBarHeight};
    SDL_SetRenderDrawColor(renderer, healthBarColor.r, healthBarColor.g, healthBarColor.b, healthBarColor.a);
    SDL_RenderFillRect(renderer, &healthBarRect);

    // Draw the mana text
    statsText = "MANA: " + to_string(player.getStat("mp")) + "/" + to_string(player.getStat("maxMp"));
    SDL_Surface* manaTextSurface = TTF_RenderText_Solid(font, statsText.c_str(), textColor);
    SDL_Texture* manaTextTexture = SDL_CreateTextureFromSurface(renderer, manaTextSurface);
    SDL_Rect manaTextRect = {STATS_X + 10, STATS_Y + 80, STATS_WIDTH - 20, 20};
    SDL_QueryTexture(manaTextTexture, nullptr, nullptr, &manaTextRect.w, &manaTextRect.h);
    SDL_RenderCopy(renderer, manaTextTexture, nullptr, &manaTextRect);
    SDL_FreeSurface(manaTextSurface);
    SDL_DestroyTexture(manaTextTexture);

    // Draw the mana bar 
    float manaPercentage = static_cast<float>(player.getStat("mp")) / player.getStat("maxMp");
    int manaBarWidth = static_cast<int>((STATS_WIDTH - 20) * manaPercentage);
    int manaBarHeight = 5;
    SDL_Rect manaBarRect = {STATS_X + 10, STATS_Y + 110, manaBarWidth, manaBarHeight};
    SDL_SetRenderDrawColor(renderer, manaBarColor.r, manaBarColor.g, manaBarColor.b, manaBarColor.a);
    SDL_RenderFillRect(renderer, &manaBarRect);

    // Draw the experience text // TODO make this a function of the draw class
    statsText = "EXP: " + to_string(player.getStat("exp")) + "/" + to_string(player.getStat("maxExp"));
    SDL_Surface* expTextSurface = TTF_RenderText_Solid(font, statsText.c_str(), textColor);
    SDL_Texture* expTextTexture = SDL_CreateTextureFromSurface(renderer, expTextSurface);
    SDL_Rect expTextRect = {STATS_X + 10, STATS_Y + 140, STATS_WIDTH - 20, 20};
    SDL_QueryTexture(expTextTexture, nullptr, nullptr, &expTextRect.w, &expTextRect.h);
    SDL_RenderCopy(renderer, expTextTexture, nullptr, &expTextRect);
    SDL_FreeSurface(expTextSurface);
    SDL_DestroyTexture(expTextTexture);

    // Draw the experience bar // TODO make this a function of the draw class
    float expPercentage = static_cast<float>(player.getStat("exp")) / player.getStat("maxExp");
    int expBarWidth = static_cast<int>((STATS_WIDTH - 20) * expPercentage);
    int expBarHeight = 5;
    SDL_Rect expBarRect = {STATS_X + 10, STATS_Y + 170, expBarWidth, expBarHeight};
    SDL_SetRenderDrawColor(renderer, expBarColor.r, expBarColor.g, expBarColor.b, expBarColor.a);
    SDL_RenderFillRect(renderer, &expBarRect);

    // Draw the level text  // TODO make this a function of the draw class
    statsText = "LVL: " + to_string(player.getStat("level"));
    SDL_Surface* levelTextSurface = TTF_RenderText_Solid(font, statsText.c_str(), textColor);
    SDL_Texture* levelTextTexture = SDL_CreateTextureFromSurface(renderer, levelTextSurface);
    SDL_Rect levelTextRect = {STATS_X + 10, STATS_Y + 200, STATS_WIDTH - 20, 20};
    SDL_QueryTexture(levelTextTexture, nullptr, nullptr, &levelTextRect.w, &levelTextRect.h);
    SDL_RenderCopy(renderer, levelTextTexture, nullptr, &levelTextRect);
    SDL_FreeSurface(levelTextSurface);
    SDL_DestroyTexture(levelTextTexture);
}

void inputHandle(bool &gameRunning, Player &player, char map[MAP_WIDTH][MAP_HEIGHT]) // TODO break this function up into smaller functions and put them in the player class/game class
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
                        if (map[player.getStat("x")][player.getStat("y") - 1] != '#') // Check if the player is trying to move into a wall TODO: Make this a function
                        {
                            player.subStat("y", 1); // Move the player TODO: Make this a function in the player class
                        }
                        update();
                        break;
                    case SDLK_DOWN:
                        if (map[player.getStat("x")][player.getStat("y") + 1] != '#')
                        {
                            player.addStat("y", 1);
                        }
                        update();
                        break;
                    case SDLK_LEFT:
                        if (map[player.getStat("x") - 1][player.getStat("y")] != '#')
                        {
                            player.subStat("x", 1);
                        }
                        update();
                        break;
                    case SDLK_RIGHT:
                        if (map[player.getStat("x") + 1][player.getStat("y")] != '#')
                        {
                            player.addStat("x", 1);
                        }
                        update();
                        break;
                    case SDLK_LALT:
                        // zoom out
                        if (TEXTURE_TILE_SIZE > 4)
                        {
                            if (mapTileSize > 1)
                            {
                                mapTileSize = mapTileSize / 2;
                            }
                        }
                        break;
                    case SDLK_LCTRL:
                        // zoom in
                        if (mapTileSize < 64)
                        {                    
                            mapTileSize = mapTileSize * 2;
                        }
                        break;
                    case SDLK_m:
                        // toggle map
                        if (mapTileSize == 8)
                        {
                            mapTileSize = 32;
                        }
                        else
                        {
                            mapTileSize = 8;
                        }
                        break;
                    case SDLK_p:
                        std::cout << "Player stats:" << endl;
                        std::cout << "x: " << player.getStat("x") << " y: " << player.getStat("y") << endl;
                        std::cout << "HP: " << player.getStat("hp") << " / " << player.getStat("maxHp") << endl;
                        std::cout << "MP: " << player.getStat("mp") << " / " << player.getStat("maxMp") << endl;
                        std::cout << "Exp: " << player.getStat("exp") << " / " << player.getStat("maxExp") << endl;
                        std::cout << "Level: " << player.getStat("level") << endl;
                    default:

                        break;
                }
                break;
        }
    }
}

void drawMiniMap(SDL_Renderer* renderer, char map[MAP_WIDTH][MAP_HEIGHT], Player player, int mapMemory[100][100]) {

    // Calculate the size of each mini-map tile
    int miniMapTileSize = 4;

    // draw the mini-map as only the 30x30 area around the player and only the tiles are in the memory map
    for (int x = player.getStat("x") - 15; x < player.getStat("x") + 15; x++) {
        for (int y = player.getStat("y") - 15; y < player.getStat("y") + 15; y++) {

            // Check if the tile is in the memory map
            bool tileInMemoryMap = false;
            for (int i = 0; i < 100; i++) {
                if (mapMemory[i][0] == x && mapMemory[i][1] == y) {
                    tileInMemoryMap = true;
                }
            }
            // If the tile is in the memory map, draw it white for open space and black for walls
            if (tileInMemoryMap) {
                if (map[x][y] == '#') {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                }
            }


            // Draw the mini-map tile
            SDL_Rect miniMapTileRect = {x * miniMapTileSize + STATS_X, y * miniMapTileSize + STATS_Y + STATS_HEIGHT, miniMapTileSize, miniMapTileSize};
            SDL_RenderFillRect(renderer, &miniMapTileRect);
        }
    }
}

int WinMain(int argc, char* argv[]) 
{
    // seedsrandd
    srand(SEED);

    // initialize the player
    Player player("NoName", 3, 3);

    // initialize SDL
    SDL_Init( SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow( "Sklernarium Top Secret Testing Program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );
    SDL_Renderer *renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

    // initialize SDL_ttf
    TTF_Init();    

    // load the tileset image to a surface
    SDL_Surface* tilesetSurface = IMG_Load("assets/tileset.png");

    // create texture from the surface
    tilesetTexture = SDL_CreateTextureFromSurface(renderer, tilesetSurface);

    // load the player image to a surface
    SDL_Surface* playerSurface = IMG_Load("assets/player.png");

    // create player texture from the surface
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);

    // Load font
    TTF_Font* font = TTF_OpenFont("assets/font.ttf", 18);
    
    // load the map from a text file // TODO: Make this a function in the game class
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

    //main game loop // TODO: Make this a function in the game class
    while (gameRunning)
    {
        // handle input
        inputHandle(gameRunning, player, map);

        updateAnimationFrame(currentFrame);

        // draw the game
        drawGame(renderer, map, player);

        // draw the mini-map
        drawMiniMap(renderer, map, player, mapMemory);

        // draw the stats 
        drawStats(renderer, font, player);

        // update the screen
        SDL_RenderPresent(renderer);

        // delay to get 45 fps
        SDL_Delay(1000 / 40);

        currentFrame++; // increment the current frame to animate the player. TODO: Make this a function in the player class called animate()

        if (currentFrame > 40)
        {
            currentFrame = 1;
        }
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