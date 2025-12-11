#include "Scene.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Color.h"
#include "Ray.h"
#include "Random.h"
#include <iostream>
#include <random>


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
				pixel += glm::vec2{ random::getReal(), random::getReal() };
				// normalize (0 <-> 1) the pixel value (pixel / vec2{ framebuffer.width, framebuffer.height }
				glm::vec2 point = pixel / glm::vec2{ framebuffer.width, framebuffer.height };
				// flip the y value (bottom = 0, top = 1)
				point.y = 1 - point.y;

				// get ray from camera
				ray_t ray = camera.GetRay(point);
				// trace ray
				color += Trace(ray, 1e-3f, 100.0f, 7); // maxDepth = 7 for 7 bounces
			}
			// get average color = (color / number samples)
			color = GetAverageColor(color, numSamples);
			framebuffer.DrawPoint(x, y, ColorConvert(color));
		}
        std::cout << y << std::endl;
	}
}

void Scene::AddObject(std::unique_ptr<Object> object) {
	objects.push_back(std::move(object));
}

color3_t Scene::Trace(const ray_t& ray, float minDistance, float maxDistance, int maxDepth) {
    if (maxDepth == 0) return { 0, 0, 0 };

    bool rayHit = false;
    float closestDistance = maxDistance;
    raycastHit_t raycastHit{};

    for (auto& object : objects) {
        if (object->Hit(ray, minDistance, closestDistance, raycastHit)) {
            rayHit = true;
            closestDistance = raycastHit.distance;
        }
    }

    if (rayHit) {
        color3_t attenuation{};
        ray_t scattered{};
        if (raycastHit.material->Scatter(ray, raycastHit, attenuation, scattered)) {
            // Normalize normal and scattered direction
            glm::vec3 n = glm::normalize(raycastHit.normal);
            glm::vec3 dir = glm::normalize(scattered.direction);
            // If direction is near-zero, fallback to normal
            if (!glm::all(glm::isfinite(dir)) || glm::length2(dir) < 1e-12f) {
                dir = n;
            }
            scattered.direction = dir;

            // Robust epsilon: relative + absolute
            constexpr float kAbsEpsilon = 1e-3f;                       // absolute bias
            float kRelEpsilon = 1e-3f * std::max(1.0f, raycastHit.distance); // scale with distance
            float eps = kAbsEpsilon + kRelEpsilon;

            // Offset along the oriented normal and along the outgoing direction
            scattered.origin = raycastHit.point + n * eps + dir * kAbsEpsilon;

            // Raise minDistance for the next trace
            float nextMin = std::max(minDistance, eps);
            return attenuation * Trace(scattered, nextMin, maxDistance, --maxDepth);
        } else {
            return raycastHit.material->GetEmissive();
        }
    }

    glm::vec3 direction = glm::normalize(glm::vec3(ray.direction));
    float t = (direction.y + 1) * 0.5f;
    color3_t color = glm::mix(skyBottom, skyTop, t);
    return color;
}
