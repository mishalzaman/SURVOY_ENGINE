#pragma once

#include <SDL2/SDL.h>
#include <string>

class Font
{
public:
	int Initialize(std::string bitmapFont, SDL_Renderer* renderer, int charW, int charH);
	void Draw(SDL_Renderer* renderer, const char* text, int x, int y, int scale);
	void Draw(SDL_Renderer* renderer, const char* text, int x, int y, int scale, SDL_Color color);

	int getCharW() { return _charW; };

private:
	SDL_Surface* _fontSurface;
	SDL_Texture* _fontTexture;
	int _charW;
	int _charH;
};

