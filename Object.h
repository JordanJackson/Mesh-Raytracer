#pragma once

#include "geometry.h"

// Base class for scene geometry
class Object
{
public:
	Object() {}
	Object(const Matrix4x4f &o2w) : objectToWorld(o2w) {}
	virtual ~Object() {}
	virtual bool Intersect(const Vec3f &, const Vec3f &, float &, uint32_t &, Vec2f &) const = 0;
	virtual void GetSurfaceProperties(const Vec3f &, const Vec3f &, const uint32_t &, const Vec2f &, Vec3f &, Vec2f &) const = 0;
	Matrix4x4f objectToWorld;
};
