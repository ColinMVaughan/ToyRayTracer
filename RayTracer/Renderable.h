#pragma once
#include "Ray.h"

class Material;

struct hit_record
{
	float t;
	vec3 p;
	vec3 normal;
	Material* mat_ptr;
};

class Renderable
{
public:
	virtual bool Render(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};


//-------------------------------------------------------------------
//
//------------------------------------------------------------------




class Sphere : public Renderable
{
public:

	Sphere() {}
	Sphere(vec3 center, float radius, Material* mat) : m_Center(center), m_Radius(radius), m_Material(mat) {}

	bool Render(const ray& r, float t_min, float t_max, hit_record& rec) const override;


private:
	vec3 m_Center;
	float m_Radius;
	Material* m_Material;
};

bool Sphere::Render(const ray& r, float t_min, float t_max, hit_record& rec) const
{
	vec3 oc = r.GetOrigin() - m_Center;
	float a = dot(r.GetDirection(), r.GetDirection());
	float b = dot(oc, r.GetDirection());
	float c = dot(oc, oc) - m_Radius * m_Radius;
	float discriminant = b * b - a*c;
	if (discriminant > 0)
	{
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.PointAtParimeter(rec.t);
			rec.normal = (rec.p - m_Center) / m_Radius;
			rec.mat_ptr = m_Material;
			return true;
		}

		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.PointAtParimeter(rec.t);
			rec.normal = (rec.p - m_Center) / m_Radius;
			rec.mat_ptr = m_Material;
			return true;
		}
	}

	return false;
}


class RenderList : public Renderable
{
public:
	RenderList() {}
	RenderList(Renderable** list, int n) { m_List = list; m_Size = n; }

	bool Render(const ray& r, float t_min, float t_max, hit_record& rec) const override;

	Renderable** m_List;
	int m_Size;
};

bool RenderList::Render(const ray& r, float t_min, float t_max, hit_record& rec) const
{
	hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = t_max;

	for (int i = 0; i < m_Size; ++i)
	{
		if (m_List[i]->Render(r, t_min, closest_so_far, temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}