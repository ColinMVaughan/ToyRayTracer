#pragma once
#include "Ray.h"
#include "Utilities.h"

#ifndef M_PI
#define M_PI 3.14159
#endif




class Camera
{
public:
	Camera(vec3 lookFrom, vec3 lookAt,vec3 up, float vfov, float aspect, float aperture, float focus_dist)
	{
		lens_radius = aperture / 2;
		float theta = vfov * M_PI / 180.0f;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;

		origin = lookFrom;
		w = unit_vector(lookFrom - lookAt);
		u = unit_vector(cross(up, w));
		v = cross(w, u);

		lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - w * focus_dist;
		horizontal = 2.0*half_width*focus_dist*u;
		vertical = 2.0*half_height*focus_dist*v;

	}

	ray GetRay(float s, float t) 
	{ 
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x + v * rd.y;
		return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset); 
	}


private:
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	float lens_radius;
};