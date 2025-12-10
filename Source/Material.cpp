#include "Material.h"
#include "Random.h"
#include <iostream>

bool Lambertian::Scatter(const ray_t& incident, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scattered) const {
    // set scattered ray using random direction from normal, diffuse the outgoing ray
    scattered.origin = raycastHit.point;
    scattered.direction = glm::normalize(raycastHit.normal + glm::vec3{ random::onUnitCircle(), 0 });

    attenuation = albedo;

    return true;
}

bool Metal::Scatter(const ray_t& incident, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scattered) const {
    glm::vec3 reflected = glm::reflect(glm::normalize(incident.direction), raycastHit.normal);

    // set scattered ray from reflected ray + random point in sphere (fuzz = 0 no randomness, fuzz = 1 random reflected)
    // a mirror has a fuzz value of 0 and a diffused metal surface a higher value
    scattered.origin = raycastHit.point;
    scattered.direction = glm::normalize(reflected + random::onUnitSphere() * fuzz);

    attenuation = albedo;

    // check that reflected ray is going away from surface normal (dot product (scattered direction, normal) > 0)
    return glm::dot(scattered.direction, raycastHit.normal) > 0;
}

// Schlick's approximation for Fresnel reflectance
static float Schlick(float cosine, float refractiveIndex) {
    float r0 = (1.0f - refractiveIndex) / (1.0f + refractiveIndex);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * std::pow((1.0f - cosine), 5.0f);
}

bool Dielectric::Scatter(const ray_t& incident, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scattered) const {
    glm::vec3 outNormal;
    float ni_over_nt;
    float cosine;

    // normalize ray direction
    glm::vec3 rayDirection = glm::normalize(incident.direction);

    // ray hits inside of surface
    if (glm::dot(incident.direction, raycastHit.normal) < 0) {
        // ray entering surface
        outNormal = raycastHit.normal;
        ni_over_nt = 1.0f / refractiveIndex;
        cosine = -glm::dot(rayDirection, raycastHit.normal);
    }
    else {
        // ray exiting surface
        outNormal = -raycastHit.normal;
        ni_over_nt = refractiveIndex;
        cosine = refractiveIndex * glm::dot(rayDirection, raycastHit.normal);
    }

    // attempt refraction
    glm::vec3 refracted = glm::refract(rayDirection, outNormal, ni_over_nt);
    float reflectProbability = 1.0f;

    // check if refraction succeeded (non-zero vector)
    if (glm::length(refracted) > 0.0f) {
        // refraction possible, use Schlick approximation
        reflectProbability = Schlick(cosine, refractiveIndex);
    }

    glm::vec3 reflected = glm::reflect(rayDirection, raycastHit.normal);

    scattered = (random::getReal() < reflectProbability) ? ray_t{ raycastHit.point, reflected } : ray_t{ raycastHit.point, refracted };
    // acts as a tint to the transparent materisl (glass)
    attenuation = albedo;
    
    return true;
}
