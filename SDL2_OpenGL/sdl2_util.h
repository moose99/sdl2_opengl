#pragma once

#include <string>
#include <iostream>

#include "SDL.h"

void logSDLError(std::ostream &os, const std::string &msg);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr);
void fill_circle(SDL_Renderer *renderer, int cx, int cy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

