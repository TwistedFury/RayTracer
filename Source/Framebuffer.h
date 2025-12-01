#pragma once
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <vector>

// framebuffer stores pixel colors in a memory buffer and uses an SDL texture to display them on screen
class Framebuffer
{
public:
	Framebuffer(const class Renderer& renderer, int width, int height);
	~Framebuffer();

	void Update();

	void Clear(const SDL_Color& color = { 0, 0, 0, 255 });
	void DrawPoint(int x, int y, const SDL_Color& color);

public:
	int width{ 0 };
	int height{ 0 };
	int pitch{ 0 };

	SDL_Texture* texture{ nullptr };
	std::vector<SDL_Color> buffer;
};