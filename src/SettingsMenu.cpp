#include "include/SettingsMenu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>


SettingsMenu::SettingsMenu(int screenWidth, int screenHeight) :
    mScreenWidth(screenWidth),
    mScreenHeight(screenHeight),
    mVisible(false),
    mEditing(false)
{
    // Set up the rectangle for the settings menu
    mRect.x = mScreenWidth / 4;
    mRect.y = mScreenHeight / 4;
    mRect.w = mScreenWidth / 2;
    mRect.h = mScreenHeight / 2;

    // Add the settings
    //screen size
    mSettings.push_back(std::make_pair("Screen Size", "800x600"));
    //player settings
    mSettings.push_back(std::make_pair("Name", "Player"));
    mSettings.push_back(std::make_pair("HP", "100"));
    mSettings.push_back(std::make_pair("MP", "100"));
    mSettings.push_back(std::make_pair("ATK", "10"));
    mSettings.push_back(std::make_pair("DEF", "10"));
    mSettings.push_back(std::make_pair("MAG", "10"));
    mSettings.push_back(std::make_pair("RES", "10"));
    mSettings.push_back(std::make_pair("SPD", "10"));
    mSettings.push_back(std::make_pair("LUK", "10"));
    mSettings.push_back(std::make_pair("EXP", "0"));
    mSettings.push_back(std::make_pair("Level", "1"));
    mSettings.push_back(std::make_pair("Gold", "0"));
    mSettings.push_back(std::make_pair("Weapon", "None"));
    mSettings.push_back(std::make_pair("Armor", "None"));
    mSettings.push_back(std::make_pair("Shield", "None"));
    mSettings.push_back(std::make_pair("Helmet", "None"));
    mSettings.push_back(std::make_pair("Accessory", "None"));
    mSettings.push_back(std::make_pair("Item 1", "None"));
    mSettings.push_back(std::make_pair("Item 2", "None"));
    mSettings.push_back(std::make_pair("Item 3", "None"));
    // debug settings
    mSettings.push_back(std::make_pair("Debug Mode", "Off"));
    mSettings.push_back(std::make_pair("God Mode", "Off"));
    mSettings.push_back(std::make_pair("No Clip", "Off"));
    mSettings.push_back(std::make_pair("Infinite Items", "Off"));
    mSettings.push_back(std::make_pair("Infinite Gold", "Off"));
    mSettings.push_back(std::make_pair("Infinite EXP", "Off"));
    mSettings.push_back(std::make_pair("Infinite HP", "Off"));
    mSettings.push_back(std::make_pair("Infinite MP", "Off"));

    
}

void SettingsMenu::show()
{
    mVisible = true;
}

void SettingsMenu::hide()
{
    mVisible = false;
}

bool SettingsMenu::isVisible()
{
    return mVisible;
}

void SettingsMenu::handleEvent(SDL_Event& event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;
        if (mouseX >= mRect.x && mouseX <= mRect.x + mRect.w && mouseY >= mRect.y && mouseY <= mRect.y + mRect.h) {
            // Clicked inside the settings menu
            int settingIndex = (mouseY - mRect.y - 20) / 20;
            if (settingIndex >= 0 && settingIndex < mSettings.size()) {
                mSelectedSetting = mSettings[settingIndex].first;
                mEditing = true;
            }
        }
    }
    else if (event.type == SDL_KEYDOWN && mEditing) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && !mSettings.empty()) {
            // Remove the last character from the selected setting
            mSettings.back().second.pop_back();
        }
        else if (event.key.keysym.sym == SDLK_RETURN) {
            // Stop editing the selected setting
            mEditing = false;
        }
        else if (event.key.keysym.sym >= SDLK_SPACE && event.key.keysym.sym <= SDLK_z && !mSettings.empty()) {
            // Add the typed character to the selected setting
            mSettings.back().second += static_cast<char>(event.key.keysym.sym);
        }
    }
}

void SettingsMenu::render(SDL_Renderer* renderer, TTF_Font* font)
{
    if (mVisible) {
        // Draw the background
        SDL_SetRenderDrawColor(renderer, 55, 55, 55, 55);
        SDL_RenderFillRect(renderer, &mRect);

        // Draw the settings
        SDL_Rect settingRect = { mRect.x + 10, mRect.y + 20, mRect.w - 20, 20 };
        for (const auto& setting : mSettings) {
            // Draw the setting name
            SDL_Surface* settingNameSurface = TTF_RenderText_Solid(font, setting.first.c_str(), SDL_Color{ 150, 150, 150 });
            SDL_Texture* settingNameTexture = SDL_CreateTextureFromSurface(renderer, settingNameSurface);
            SDL_Rect settingNameRect = { settingRect.x, settingRect.y, 100, 20 };
            SDL_RenderCopy(renderer, settingNameTexture, nullptr, &settingNameRect);
            SDL_FreeSurface(settingNameSurface);
            SDL_DestroyTexture(settingNameTexture);

            // Draw the setting value
            SDL_Surface* settingValueSurface = TTF_RenderText_Solid(font, setting.second.c_str(), SDL_Color{ 50, 50, 255 });
            SDL_Texture* settingValueTexture = SDL_CreateTextureFromSurface(renderer, settingValueSurface);
            SDL_Rect settingValueRect = { settingRect.x + 110, settingRect.y, settingRect.w - 120, 20 };
            SDL_RenderCopy(renderer, settingValueTexture, nullptr, &settingValueRect);
            SDL_FreeSurface(settingValueSurface);
            SDL_DestroyTexture(settingValueTexture);

            settingRect.y += 20;
        }

        // Draw the selected setting if editing
        if (mEditing) {
            SDL_Rect selectedSettingRect = { mRect.x + 10, mRect.y + 20 + (20 * (mSettings.size() - 1)), mRect.w - 20, 20 };
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &selectedSettingRect);
            SDL_Surface* selectedSettingSurface = TTF_RenderText_Solid(font, mSelectedSetting.c_str(), SDL_Color{ 255, 255, 255 });
            SDL_Texture* selectedSettingTexture = SDL_CreateTextureFromSurface(renderer, selectedSettingSurface);
            SDL_Rect selectedSettingTextRect = { selectedSettingRect.x + 5, selectedSettingRect.y + 2, selectedSettingRect.w - 10, selectedSettingRect.h - 4 };
            SDL_RenderCopy(renderer, selectedSettingTexture, nullptr, &selectedSettingTextRect);
            SDL_FreeSurface(selectedSettingSurface);
            SDL_DestroyTexture(selectedSettingTexture);
        }
    }
}