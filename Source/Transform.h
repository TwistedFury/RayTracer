#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform {
    glm::vec3 position{ 0, 0, 0 };

    Transform() = default;
    Transform(const glm::vec3& position) :
        position{ position }
    {}
};