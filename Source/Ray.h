#pragma once

#include <glm/glm.hpp>

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;

	Ray() = default;
	Ray(const glm::vec3& ori, const glm::vec3& dir) :
		origin{ori}, direction{dir} 
	{}

	glm::vec3 at(float t) const { return origin + (t * direction); }

	// Overload * (at)
	glm::vec3 operator * (float t) { return at(t); }
};