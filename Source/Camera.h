#pragma once
#include "Ray.h"
#include <glm/glm.hpp>

class Camera
{
public:
	Camera(float fov, float aspectRatio) : fov{ fov }, aspectRatio{ aspectRatio } {}

	void SetView(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up = glm::vec3{ 0, 1, 0 });
	void SetFOV(float fov) { this->fov = fov; CalculateViewPlane(); }

	// get ray from point on the view plane
	ray_t GetRay(const glm::vec2& uv) const;

private:
	void CalculateViewPlane();

private:
	float fov{ 60 }; // fov in degrees
	float aspectRatio{ 1 }; // screen width / screen height

	glm::vec3 eye{ 0 };

	// camera axis
	glm::vec3 forward{ 0 };
	glm::vec3 right{ 0 };
	glm::vec3 up{ 0 };

	// view plane origin and horizontal and vertical direction vectors
	glm::vec3 lowerLeft{ 0 };
	glm::vec3 horizontal{ 0 };
	glm::vec3 vertical{ 0 };
};