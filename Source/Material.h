#pragma once

#include "Color.h"
#include "Ray.h"

class Material
{
public:
	Material() = default;
	Material(const color3_t& albedo) : albedo{ albedo } {}
	virtual ~Material() = default;

	// computes material response to incident ray: returns scattered ray direction and attenuation color. 
	// returns false if ray is absorbed (e.g., emissive materials).
	virtual bool Scatter(const ray_t& incident, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scattered) const = 0;

	const color3_t& GetColor() const { return albedo; }
	virtual color3_t GetEmissive() const { return color3_t{ 0, 0, 0 }; }

protected:
	color3_t albedo{ 0, 0, 0 }; // surface color
};

// diffuse material: rays scatter in all directions
class Lambertian : public Material
{
public:
	Lambertian(const color3_t& albedo) : Material{ albedo } {}

	bool Scatter(const ray_t& incident, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scattered) const override;
};

// shiny material: rays are reflected off of surface, fuzz controls how mirror like the material is
class Metal : public Material
{
public:
	Metal(const glm::vec3& albedo, float fuzz) : Material{ albedo }, fuzz{ std::clamp(fuzz, 0.0f, 1.0f) } {}

	bool Scatter(const ray_t& incident, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scattered) const override;

private:
	float fuzz = 0; // 0 is "perfect" reflection (mirror), higher values randomize reflection (1 = diffused metal)
};

// transparent material: rays are refracted (bent) as they pass through the material
class Dielectric : public Material
{
public:
	Dielectric(const glm::vec3& albedo, float refractiveIndex) : Material{ albedo }, refractiveIndex{ std::max(refractiveIndex, 1.0f) } {}

	bool Scatter(const ray_t& incident, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scattered) const override;

private:
	float refractiveIndex = 0;
};

// light emitting material: rays are absorbed by light and attenuation color is scaled with intensity (brightness)
class Emissive : public Material
{
public:
	Emissive(const color3_t& albedo, float intensity = 1) : Material{ albedo }, intensity{ intensity } { }

	// ray that hits emmissive material isn't scattered (gets absorbed)
	bool Scatter(const ray_t& incident, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scattered) const override { return false; }
	
	color3_t GetEmissive() const override { return albedo * intensity; }

private:
	float intensity{ 1 }; // scale albedo color with intensity, the larger the value the brighter
};