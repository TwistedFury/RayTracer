#include "Sphere.h"
#include <cmath>

bool Sphere::Hit(const ray_t& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) {
    glm::vec3 oc = ray.origin - transform.position;

    float a = glm::dot(ray.direction, ray.direction);
    float b = 2 * glm::dot(ray.direction, oc);
    float c = glm::dot(oc, oc) - (radius * radius);

    float discriminant = (b * b) - (4 * a * c);
    if (discriminant < 0) return false;

    float sqrtD = std::sqrt(discriminant);

    auto setHit = [&](float t) {
        raycastHit.distance = t;
        raycastHit.point = ray.at(t);
        // Geometric normal
        glm::vec3 outwardNormal = (raycastHit.point - transform.position) / radius;
        outwardNormal = glm::normalize(outwardNormal);

        // Orient normal to oppose the ray (front-face)
        bool frontFace = glm::dot(ray.direction, outwardNormal) < 0.0f;
        raycastHit.normal = frontFace ? outwardNormal : -outwardNormal;

        raycastHit.color = material->GetColor();
        raycastHit.material = material; // shared_ptr
    };

    float t = (-b - sqrtD) / (2 * a);
    constexpr float kEps = 1e-4f;
    if (t > minDistance + kEps && t < maxDistance) { setHit(t); return true; }
    t = (-b + sqrtD) / (2 * a);
    if (t > minDistance + kEps && t < maxDistance) { setHit(t); return true; }

    return false;
}
