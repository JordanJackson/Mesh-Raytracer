#pragma once

#include "Vec2.h"
#include "Vec3.h"
#include "Matrix4x4.h"

static const double PI = 3.14159265358979323846;
static const double PI_2 = 1.57079632679489661923;

static const float kInfinity = std::numeric_limits<float>::max();
static const float kEpsilon = 1e-8;

inline float clamp(const float &lo, const float &hi, const float &v)
{
	return std::max(lo, std::min(hi, v));
}

inline float deg2rad(const float &deg)
{
	return deg * PI / 180;
}


// Moller-Trumbore Algorithm
bool rayTriangleIntersect(const Vec3f &origin, const Vec3f &direction,
	const Vec3f &point0, const Vec3f &point1, const Vec3f &point2,
	float &t,
	float &u, float &v)
{
	Vec3f edge0_1 = point1 - point0;
	Vec3f edge1_2 = point2 - point0;
	Vec3f perpVec = direction.CrossProduct(edge1_2);
	float det = edge0_1.DotProduct(perpVec);

	// Ray and Triangle are parallel if determinant is close to 0
	if (fabs(det) < kEpsilon) return false;
	// Pre-calculate inverse determinant
	float invDet = 1 / det;

	Vec3f originToPlaneVec = origin - point0;
	u = originToPlaneVec.DotProduct(perpVec) * invDet;
	if (u < 0 || u > 1) return false;

	Vec3f qVec = originToPlaneVec.CrossProduct(edge0_1);
	v = direction.DotProduct(qVec) * invDet;
	if (v < 0 || u + v > 1) return false;

	t = edge1_2.DotProduct(qVec) * invDet;

	return true;
}