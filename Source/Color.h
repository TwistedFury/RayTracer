#pragma once
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <algorithm>

// use alias to clarify type name
using color3_t = glm::vec3;
using color4_t = glm::vec4;

// convert from RGBA(0.0 - 1.0) color to (0 - 255) color
inline SDL_Color ColorConvert(const color4_t& color4)
{
	SDL_Color color;

	color.r = 0; // convert color4 to SDL_Color color
	color.g = 0; // convert color4 to SDL_Color color
	color.b = 0; // convert color4 to SDL_Color color
	color.a = 0; // convert color4 to SDL_Color color

	return color;
}

// convert from RGB(0.0 - 1.0) color to (0 - 255) color
inline SDL_Color ColorConvert(const color3_t& color3)
{
	SDL_Color color;

	color.r = 0; // convert color4 to SDL_Color color
	color.g = 0; // convert color4 to SDL_Color color
	color.b = 0; // convert color4 to SDL_Color color
	color.a = 255;

	return color;
}

// convert from (0 - 255) color to RGBA(0.0 - 1.0) color
inline color4_t ColorConvert(const SDL_Color& color)
{
	color4_t color4;

	color4.r = 0; // convert SDL_Color color to color4
	color4.g = 0; // convert SDL_Color color to color4
	color4.b = 0; // convert SDL_Color color to color4
	color4.a = 0; // convert SDL_Color color to color4

	return color4;
}
