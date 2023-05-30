#ifndef GAME_H
#define GAME_H

class Game {
    private:

    public:
        bool running;
        Game();
        ~Game();

        void run(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, SDL_Texture* tilesetTexture, SDL_Texture* playerTexture, SDL_Texture* mobTexture, SDL_Texture* fontTexture, char map[100][100]);
        void drawTile(SDL_Renderer* renderer, char tileType, int x, int y);
        void drawGame(SDL_Renderer* renderer, char map[MAP_WIDTH][MAP_HEIGHT]);
        void drawStats(SDL_Renderer* renderer);
        void updateMemory();
        void inputHandle();
        void handleAI();
};

#endif