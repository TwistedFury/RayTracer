#include "Plane.h"

bool Plane::Hit(const ray_t& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) {
    float t;
    glm::vec3 center = transform.position;
    glm::vec3 normal = transform.Up();
    // check ray intersection, returns true if ray intersects, t is distance to intersection
    if (!Raycast(ray, center, normal, minDistance, maxDistance, t)) return false;

    // set raycast parameters
    raycastHit.distance = t;
    raycastHit.point = ray.at(t);
    raycastHit.normal = normal;
    raycastHit.material = material;

    return true;
}

bool Plane::Raycast(const ray_t& ray, const glm::vec3& point, const glm::vec3& normal, float minDistance, float maxDistance, float& t)
{
    // check dot product of ray direction and plane normal, if result is 0 then ray direction is parallel to plane
    // the dot product is 0 if the two vectors are perpendicular (90 degrees)
    float denominator = glm::dot(ray.direction, normal); // dot product between ray.direction and normal
    if (std::abs(denominator) < glm::epsilon<float>()) {
        return false; // ray direction and plane parallel, no intersection
    }

    // Solve for t in the ray–plane intersection equation:
    //     (planePoint - rayOrigin) · planeNormal
    // t = ----------------------------------------
    //           rayDirection · planeNormal
    //
    // t represents the distance along the ray where it hits the plane.
    // A positive t means the plane is in front of the ray; negative t
    // means the ray is pointing away from the plane.
    t = glm::dot((point - ray.origin), normal) / denominator;
    constexpr float kEps = 1e-4f;
    // return true if within distance bounds
    if (t < maxDistance && t > minDistance + kEps) return true;
    return false;
}
