#pragma once

#include "Color.h"
#include <glm/glm.hpp>
#include <memory>
// Forward declaration to avoid circular dependency with Material.h
class Material;

struct ray_t
{
	glm::vec3 origin;
	glm::vec3 direction;

	ray_t() = default;
	ray_t(const glm::vec3& ori, const glm::vec3& dir) :
		origin{ori}, direction{dir} 
	{}

	glm::vec3 at(float t) const { return origin + (t * direction); }

	// Overload * (at)
	glm::vec3 operator * (float t) { return at(t); }
};

struct raycastHit_t
{
	glm::vec3 point;	// point of ray hit
	glm::vec3 normal;	// normal of surface hit
	float distance;	// distance from ray origin to hit
    color3_t color;    // surface color at hit

	std::shared_ptr<Material> material;	// hit object material
};