#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Game.h"


Game::Game() {
    running = true;
}  

void Game::run(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, SDL_Texture* tilesetTexture, SDL_Texture* playerTexture, SDL_Texture* mobTexture, SDL_Texture* fontTexture, char map[100][100]) {
 // seed the random number generator
    srand(time(NULL));
    // initialize SDL
    SDL_Init( SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow( "Roguelike", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 640, SDL_WINDOW_SHOWN );
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
        for (int y = 0; y < 100; y++)
        {
            for (int x = 0; x < 100; x++)
            {
                mapFile >> map[x][y];
            }
        }
    }
    mapFile.close();

    //main game loop
    while (running)
    {
        // handle input
        inputHandle();

        // draw the game
        drawGame(renderer, map);
        // delay to get 60 fps
        SDL_Delay(1000 / 60);
    }
}
