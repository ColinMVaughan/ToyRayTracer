#pragma once
#include "Vectors.h"

using namespace RayMath;

class ray
{
public:
	ray() {};
	ray(const vec3& origin, const vec3& direction)
	: m_Origin(origin), m_Direction(direction) {}

	vec3 GetOrigin() const { return m_Origin; }
	vec3 GetDirection() const { return m_Direction; }
	vec3 PointAtParimeter(float t) const { return m_Origin + t * m_Direction; }



	vec3 m_Origin;
	vec3 m_Direction;
};