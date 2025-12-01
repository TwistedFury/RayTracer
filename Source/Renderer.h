#pragma once
#include <SDL3/SDL.h>
#include <string>

class Renderer
{
public:
	Renderer() = default;
	~Renderer();

	bool Initialize();
	bool CreateWindow(const std::string& name, int width, int height, bool fullscreen = false);
	void Show();

	void CopyFramebuffer(const class Framebuffer& framebuffer);

public:
	int width{ 0 };
	int height{ 0 };

	SDL_Window* window{ nullptr };
	SDL_Renderer* renderer{ nullptr };
};