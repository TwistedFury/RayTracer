#pragma once

#include "Object.h"

class Sphere : public Object
{
public:
	Sphere() = default;
	Sphere(const glm::vec3& position, float radius, const color3_t& color) :
		Object(color),
		position{position},
		radius{radius}
	{ }

	bool Hit(const ray_t& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) override;

public:
	glm::vec3 position;
	float radius{ 0 };
};