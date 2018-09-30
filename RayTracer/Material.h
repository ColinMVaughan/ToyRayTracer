#pragma once
#include "Ray.h"
#include "Renderable.h"
#include "Utilities.h"

//--------------------------------------------------------------------------------------------------------------
class Material
{
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

class Lambertian : public Material
{
public:
	Lambertian(const vec3& a) :m_Albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
	{
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = m_Albedo;
		return true;
	}

	vec3 m_Albedo;
};

class Metal : public Material
{
public:
	Metal(const vec3& a, float f):m_Albedo(a), m_FuzzFactor(f){}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
	{
		vec3 reflected = reflect(unit_vector(r_in.GetDirection()), rec.normal);
		scattered = ray(rec.p, reflected + m_FuzzFactor*random_in_unit_sphere());
		attenuation = m_Albedo;
		return (dot(scattered.GetDirection(), rec.normal) > 0);
	}
	vec3 m_Albedo;
	float m_FuzzFactor;
};

class Dielectric :public Material
{
public:
	Dielectric(float refIndex): m_RefIndex(refIndex) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
	{
		vec3 outward_normal;
		vec3 reflected = reflect(r_in.GetDirection(), rec.normal);
		vec3 refracted;

		float ni_over_nt;
		float reflect_prob;
		float cosine;
		attenuation = vec3(1.0f, 1.0f, 1.0f);

		if (dot(r_in.GetDirection(), rec.normal) > 0)
		{
			outward_normal = -rec.normal;
			ni_over_nt = m_RefIndex;
			cosine = m_RefIndex * dot(r_in.GetDirection(), rec.normal) / r_in.GetDirection().length();
		}
		else
		{
			outward_normal = rec.normal;
			ni_over_nt = 1.0f / m_RefIndex;
			cosine = -dot(r_in.GetDirection(), rec.normal) / r_in.GetDirection().length();
		}

		if (refract(r_in.GetDirection(), outward_normal, ni_over_nt, refracted))
		{
			reflect_prob = schlick(cosine, m_RefIndex);
		}
		else
		{
			reflect_prob = 1.0f;
		}

		if (RandomRange() < reflect_prob)
			scattered = ray(rec.p, reflected);
		else
			scattered = ray(rec.p, refracted);

		return true;
	}

	float m_RefIndex;
};