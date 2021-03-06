#pragma once
#include <math.h>
#include <stdlib.h>
#include <iostream>

namespace RayMath
{

	class vec3
	{
	public:
		vec3() { e[0] = 0.0f; e[1] = 0.0f; e[2] = 0.0f; };
		vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }


		inline const vec3& operator+() const { return *this; }
		inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
		inline float operator[](int i) const { return e[i]; }
		inline float& operator[](int i) { return e[i]; }

		inline vec3& operator+=(const vec3& v2);
		inline vec3& operator-=(const vec3& v2);
		inline vec3& operator*=(const vec3& v2);
		inline vec3& operator/=(const vec3& v2);
		inline vec3& operator*=(const float t);
		inline vec3& operator/=(const float t);

		inline float length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
		inline float squared_length() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
		inline void make_unit_vector();
		
		union
		{
			float e[3];
			struct { float x, y, z; };
			struct { float r, g, b; };

		};

	};



}
	//------------------------------------------------------------------------------------------------------------

inline std::istream& operator >> (std::istream& is, RayMath::vec3 t)
{
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}

inline std::ostream& operator << (std::ostream& os, RayMath::vec3 t)
{
	os << t.e[0] << t.e[1] << t.e[2];
	return os;
}


inline RayMath::vec3 operator+(const RayMath::vec3& v1, const RayMath::vec3& v2)
{
	return RayMath::vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline RayMath::vec3 operator-(const RayMath::vec3& v1, const RayMath::vec3& v2)
{
	return RayMath::vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline RayMath::vec3 operator*(const RayMath::vec3& v1, const RayMath::vec3& v2)
{
	return RayMath::vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline RayMath::vec3 operator/(const RayMath::vec3& v1, const RayMath::vec3& v2)
{
	return RayMath::vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}


inline RayMath::vec3 operator*(const RayMath::vec3& v1, float t)
{
	return RayMath::vec3(v1.e[0] * t, v1.e[1] * t, v1.e[2] * t);
}

inline RayMath::vec3 operator/(const RayMath::vec3& v1, float t)
{
	return RayMath::vec3(v1.e[0] / t, v1.e[1] / t, v1.e[2] / t);
}
inline RayMath::vec3 operator*(float t, const RayMath::vec3& v1)
{
	return RayMath::vec3(t * v1.e[0], t * v1.e[1], t * v1.e[2]);
}


	//------------------------------------------------------------------------------------------------------------

namespace RayMath
{

	inline vec3 & RayMath::vec3::operator+=(const vec3 & v2)
	{
		e[0] += v2.e[0];
		e[1] += v2.e[1];
		e[2] += v2.e[2];

		return *this;
	}

	inline vec3 & RayMath::vec3::operator-=(const vec3 & v2)
	{
		e[0] -= v2.e[0];
		e[1] -= v2.e[1];
		e[2] -= v2.e[2];

		return *this;
	}

	inline vec3 & RayMath::vec3::operator*=(const vec3 & v2)
	{
		e[0] *= v2.e[0];
		e[1] *= v2.e[1];
		e[2] *= v2.e[2];

		return *this;
	}

	inline vec3 & RayMath::vec3::operator/=(const vec3 & v2)
	{
		e[0] /= v2.e[0];
		e[1] /= v2.e[1];
		e[2] /= v2.e[2];

		return *this;
	}

	inline vec3 & RayMath::vec3::operator*=(const float t)
	{
		e[0] *=t;
		e[1] *=t;
		e[2] *=t;

		return *this;
	}

	inline vec3 & RayMath::vec3::operator/=(const float t)
	{
		float k = 1.0 / t;

		e[0] *= k;
		e[1] *= k;
		e[2] *= k;

		return *this;
	}

	inline void RayMath::vec3::make_unit_vector()
	{
		float k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
		e[0] *= k; e[1] *= k; e[2] *= k;
	}
	

	inline float dot(const vec3 &v1, const vec3 &v2)
	{
		return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
	}

	inline vec3 cross(const vec3 &v1, const vec3 &v2)
	{
		return vec3((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
			(-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
			(v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
	}

	inline vec3 unit_vector(vec3 v)
	{
		return v / v.length();
	}

}

