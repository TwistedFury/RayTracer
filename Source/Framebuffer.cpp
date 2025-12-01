#include "Framebuffer.h"
#include "Renderer.h"
#include <iostream>

Framebuffer::Framebuffer(const Renderer& renderer, int width, int height) {
	// store framebuffer parameters
	this->width = width;
	this->height = height;
	// pitch is the size of a row in bytes
	this->pitch = width * sizeof(SDL_Color);

	// create texture in RGBA (8888) format
	texture = SDL_CreateTexture(renderer.renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
	if (texture == nullptr)
	{
		std::cerr << "Error creating SDL texture: " << SDL_GetError() << std::endl;
	}

	// resize buffer to size of framebuffer (width * height)
	buffer.resize(width * height);
}

Framebuffer::~Framebuffer() {
	SDL_DestroyTexture(texture);
}

void Framebuffer::Update() {
	// write pixel colors in the buffer to the texture
	SDL_UpdateTexture(texture, NULL, buffer.data(), pitch);
}

void Framebuffer::Clear(const SDL_Color& color) {
	std::fill(buffer.begin(), buffer.end(), color);
}

void Framebuffer::DrawPoint(int x, int y, const SDL_Color& color) {
	// check for out of bounds
	if (x < 0 || x >= width || y < 0 || y >= height) {
		std::cerr << "pixel out of bounds - x: " << x << " y: " << y << ".\n";
		return;
	}

	buffer[x + (y * width)] = color;
}

