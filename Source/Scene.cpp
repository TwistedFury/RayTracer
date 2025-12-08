#include "Scene.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Color.h"
#include "Ray.h"
#include <iostream>
#include <random>

// Helper function for random float in [0, 1)
inline float getRandomFloat()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    return dis(gen);
}

void Scene::Render(Framebuffer& framebuffer, const Camera& camera, int numSamples) {
	// trace ray for every framebuffer pixel
	for (int y = 0; y < framebuffer.height; y++) {
		for (int x = 0; x < framebuffer.width; x++) {
			// color will be accumulated with ray trace samples
			color3_t color{ 0 };
			// multi-sample for each pixel
			for (int i = 0; i < numSamples; i++) {
				// set pixel (x,y) coordinates)
				glm::vec2 pixel{ x, y };
				// add random value (0-1) to pixel valie, each sample should be a little different
				pixel += glm::vec2{ getRandomFloat(), getRandomFloat() };
				// normalize (0 <-> 1) the pixel value (pixel / vec2{ framebuffer.width, framebuffer.height }
				glm::vec2 point = pixel / glm::vec2{ framebuffer.width, framebuffer.height };
				// flip the y value (bottom = 0, top = 1)
				point.y = 1 - point.y;

				// get ray from camera
				ray_t ray = camera.GetRay(point);
				raycastHit_t raycastHit;
				// trace ray
				color += Trace(ray, 0, 100, raycastHit);
			}
			// get average color = (color / number samples)
			color = color3_t{ 0.5f, 0.5f, 0.5f };
			framebuffer.DrawPoint(x, y, ColorConvert(color));
		}
	}
}

void Scene::AddObject(std::unique_ptr<Object> object) {
	objects.push_back(std::move(object));
}

color3_t Scene::Trace(const ray_t& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) {

	bool rayHit = false;
	float closestDistance = maxDistance;

	// check if scene objects are hit by the ray
	for (auto& object : objects) {
		// when checking objects don't include objects farther than closest hit (starts at max distance)
		if (object->Hit(ray, minDistance, closestDistance, raycastHit))	{
			rayHit = true;
			// set closest distance to the raycast hit distance (only hit objects closer than closest distance)
			closestDistance = raycastHit.distance;
		}
	}

	// check if ray hit object
	if (rayHit)	{
		// get material color of hit object
		color3_t color = raycastHit.color;
		return color;
	}

	// draw sky colors based on the ray y position
	glm::vec3 direction = glm::normalize(glm::vec3(ray.direction));
	// shift direction y from -1 <-> 1 to 0 <-> 1
	float t = (direction.y + 1) * 0.5f;
	
	// interpolate between sky bottom (0) to sky top (1)
	color3_t color = glm::mix(skyBottom, skyTop, t);

	return color;
}
