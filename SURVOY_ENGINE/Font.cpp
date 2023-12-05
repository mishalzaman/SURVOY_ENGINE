#include "Font.h"
#include "Code.h"

int Font::Initialize(std::string bitmapFont, SDL_Renderer* renderer, int charW, int charH)
{
    _charW = charW;
    _charH = charH;

    _fontSurface = SDL_LoadBMP("assets/font.bmp");
    if (_fontSurface == NULL) {
        return ENGINE::Code::FONT_NOT_LOADED;
    }
    _fontTexture = SDL_CreateTextureFromSurface(renderer, _fontSurface);

    return ENGINE::Code::CORE_SUCCESS;
}

void Font::Draw(SDL_Renderer* renderer, const char* text, int x, int y, int scale)
{
    int length = strlen(text);
    SDL_SetTextureColorMod(_fontTexture, 255, 255, 255);

    for (int i = 0; i < length; ++i) {
        char c = text[i] - 32;  // Assuming your bitmap font starts with space (ASCII 32)
        c = std::tolower(c);
        int row = c / 16; // Add 2 to offset the empty rows
        int col = c % 16;

        SDL_Rect srcRect = { col * _charW, row * _charH, _charW, _charH };

        int dX = x + i * _charW * scale;
        int dY = y;
        int dW = _charW * scale;
        int dH = _charH * scale;
        if (dX > 1024) {
            dX = 0 + i * _charW * scale;
            dY = dY + 64;
        }

        SDL_Rect dstRect = { dX, dY, dW, dH };

        SDL_RenderCopy(renderer, _fontTexture, &srcRect, &dstRect);
    }
}

void Font::Draw(SDL_Renderer* renderer, const char* text, int x, int y, int scale, SDL_Color color)
{
    int length = strlen(text);
    SDL_SetTextureColorMod(_fontTexture, color.r, color.b, color.g);

    for (int i = 0; i < length; ++i) {
        char c = text[i] - 32;  // Assuming your bitmap font starts with space (ASCII 32)
        c = std::tolower(c);
        int row = c / 16; // Add 2 to offset the empty rows
        int col = c % 16;

        SDL_Rect srcRect = { col * _charW, row * _charH, _charW, _charH };

        int dX = x + i * _charW * scale;
        int dY = y;
        int dW = _charW * scale;
        int dH = _charH * scale;
        if (dX > 1024) {
            dX = 0 + i * _charW * scale;
            dY = dY + 64;
        }

        SDL_Rect dstRect = { dX, dY, dW, dH };

        SDL_RenderCopy(renderer, _fontTexture, &srcRect, &dstRect);
    }
}
