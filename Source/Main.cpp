#include "Renderer.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Scene.h"
#include "Sphere.h"
#include "Random.h"

#include <iostream>

int main() {
	constexpr int SCREEN_WIDTH = 800;
	constexpr int SCREEN_HEIGHT = 600;

	// create renderer
	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("Ray Tracer", SCREEN_WIDTH, SCREEN_HEIGHT);

	Framebuffer framebuffer(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	float aspectRatio = ((float)framebuffer.width / framebuffer.height);
	Camera camera(70.0f, aspectRatio);
	camera.SetView({ 0, 0, 5 }, { 0, 0, 0 });

	Scene scene;
	scene.SetSky({ 1, 1, 1 }, { 0, 0, 0 });

	auto red = std::make_shared<Lambertian>(color3_t{ 1.0f, 0.0f, 0.0f });
	auto green = std::make_shared<Lambertian>(color3_t{ 0.0f, 1.0f, 0.0f });
	auto blue = std::make_shared<Lambertian>(color3_t{ 0.0f, 0.0f, 1.0f });
	auto light = std::make_shared<Emissive>(color3_t{ 1.0f, 1.0f, 1.0f }, 3.0f);
	auto metal = std::make_shared<Metal>(color3_t{ 1.0f, 1.0f, 1.0f }, 0.0f);
	std::vector<std::shared_ptr<Material>> materials = {red, green, blue, light, metal};

	for (int i = 0; i < 15; i++) {
		glm::vec3 position = random::getReal(glm::vec3{ -3.0f }, glm::vec3{ 3.0f });

		std::unique_ptr<Object> sphere = std::make_unique<Sphere>(Transform{ position }, random::getReal(0.2f, 1.0f), materials[random::getInt(materials.size())]);
		scene.AddObject(std::move(sphere));
	}

	scene.Render(framebuffer, camera, 500);

	SDL_Event event;
	bool quit = false;
	while (!quit) {
		// check for exit events
		while (SDL_PollEvent(&event)) {
			// window (X) quit
			if (event.type == SDL_EVENT_QUIT) {
				quit = true;
			}
			// escape key quit
			if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE) {
				quit = true;
			}
		}

		// draw to frame buffer
		//framebuffer.Clear({ 0, 0, 0, 255 });
		// scene Render

		// update frame buffer, copy buffer pixels to texture
		framebuffer.Update();

		// copy frame buffer texture to renderer to display
		renderer.CopyFramebuffer(framebuffer);
		renderer.Show();
	}
}