#pragma once 

#ifndef SETTINGS_MENU_H
#define SETTINGS_MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

class SettingsMenu
{
    private:
        int mScreenWidth;
        int mScreenHeight;
        bool mVisible;
        bool mEditing;
        std::string mSelectedSetting;
        std::vector<std::pair<std::string, std::string>> mSettings;
        SDL_Rect mRect;
    public:
        SettingsMenu( int screenWidth, int screenHeight);
        void show();
        void hide();
        bool isVisible();
        void handleEvent(SDL_Event& event);
        void render(SDL_Renderer* renderer, TTF_Font* font);
};

#endif