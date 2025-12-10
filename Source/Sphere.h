#pragma once

#include "Object.h"

class Sphere : public Object
{
public:
	Sphere() = default;
	Sphere(const Transform& transform, float radius, const std::shared_ptr<Material> material) :
		Object(transform, material),
		radius{ radius }
	{ }

	bool Hit(const ray_t& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) override;

public:
	float radius{ 0 };
};