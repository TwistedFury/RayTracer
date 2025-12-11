#pragma once
#include "Object.h"

class Plane : public Object
{
public:
	Plane() = default;
	Plane(const Transform& transform, std::shared_ptr<Material> material) :
		Object{ transform, material }		
	{}

	bool Hit(const ray_t& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) override;

	// check ray to plane intersection, returns true if ray intersects, t is distance to intersection
	static bool Raycast(const ray_t& ray, 
									 const glm::vec3& point, 
									 const glm::vec3& normal, 
									 float minDistance, 
									 float maxDistance, 
									 float& t);
};