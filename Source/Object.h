#pragma once
#include "Color.h"
#include "Ray.h"
#include "Material.h"
#include "Transform.h"

#include <memory>

class Object
{
public:
	Object() = default;
	Object(const Transform& transform, std::shared_ptr<Material> material) :
		transform{ transform },
		material{ material }
	{
	}

	virtual bool Hit(const ray_t& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) = 0;

protected:
	Transform transform;
	std::shared_ptr<Material> material;
};