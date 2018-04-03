#pragma once

#include <memory>

#include "MathHeader.h"
#include "Object.h"

#define MT_ALGO true;

class TriangleMesh : public Object
{
	// member variables
	uint32_t numTris;
	std::unique_ptr<Vec3f[]> positions;
	std::unique_ptr<uint32_t[]> indices;
	std::unique_ptr<Vec3f[]> normals;
	std::unique_ptr<Vec2f[]> texCoords;

public:
	// Build a triangle mesh from a face index array and a vertex index array
	TriangleMesh(
		const uint32_t nFaces,
		const std::unique_ptr<uint32_t[]> &faceIndex,
		const std::unique_ptr<uint32_t[]> &vertsIndex,
		const std::unique_ptr<Vec3f[]> &verts,
		std::unique_ptr<Vec3f[]> &n,
		std::unique_ptr<Vec2f[]> &st) : numTris(0)
	{
		//uint32_t k = 0, maxVertexIndex = 0;
		//// determine number of triangles in mesh
		//for (uint32_t i = 0; i < nFaces; ++i)
		//{
		//	numTris += faceIndex[i] - 2;
		//	for (uint32_t j = 0; j < faceIndex[i]; ++j)
		//	{
		//		uint32_t current = vertsIndex[k + j];
		//		if (current > maxVertexIndex)
		//			maxVertexIndex = current;
		//	}
		//	k += faceIndex[i];
		//}
		//maxVertexIndex += 1; // count = index + 1

		//// allocate memory to store the position of the mesh vertices
		//positions = std::unique_ptr<Vec3f[]>(new Vec3f[maxVertexIndex]);
		//for (uint32_t i = 0; i < maxVertexIndex; ++i)
		//{
		//	positions[i] = verts[i];
		//}
		//// allocate memory to store triangle indices
		//indices = std::unique_ptr<uint32_t[]>(new uint32_t[numTris * 3]);
		//uint32_t l = 0;
		//// generate triangle index array
		//normals = std::unique_ptr<Vec3f[]>(new Vec3f[numTris * 3]);
		//texCoords = std::unique_ptr<Vec2f[]>(new Vec2f[numTris * 3]);
		//// for each face
		//for (uint32_t i = 0, k = 0; i < nFaces; ++i)
		//{
		//	// for each triangle in the face
		//	for (uint32_t j = 0; j < faceIndex[i] - 2; ++j)
		//	{
		//		indices[l] = vertsIndex[k];
		//		indices[l + 1] = vertsIndex[k + j + 1];
		//		indices[l + 2] = vertsIndex[k + j + 2];
		//		normals[l] = n[k];
		//		normals[l + 1] = n[k + j + 1];
		//		normals[l + 2] = n[k + j + 2];
		//		texCoords[l] = st[k];
		//		texCoords[l + 1] = st[k + j + 1];
		//		texCoords[l + 2] = st[k + j + 2];
		//		l += 3;
		//	}
		//	k += faceIndex[i];
		//}
		uint32_t k = 0, maxVertIndex = 0;
		// find out how many triangles we need to create for this mesh
		for (uint32_t i = 0; i < nFaces; ++i) {
			numTris += faceIndex[i] - 2;
			for (uint32_t j = 0; j < faceIndex[i]; ++j)
				if (vertsIndex[k + j] > maxVertIndex)
					maxVertIndex = vertsIndex[k + j];
			k += faceIndex[i];
		}
		maxVertIndex += 1;

		// allocate memory to store the position of the mesh vertices
		positions = std::unique_ptr<Vec3f[]>(new Vec3f[maxVertIndex]);
		for (uint32_t i = 0; i < maxVertIndex; ++i) {
			positions[i] = verts[i];
		}

		// allocate memory to store triangle indices
		indices = std::unique_ptr<uint32_t[]>(new uint32_t[numTris * 3]);
		uint32_t l = 0;
		// [comment]
		// Generate the triangle index array
		// Keep in mind that there is generally 1 vertex attribute for each vertex of each face.
		// So for example if you have 2 quads, you only have 6 vertices but you have 2 * 4
		// vertex attributes (that is 8 normals, 8 texture coordinates, etc.). So the easiest
		// lazziest method in our triangle mesh, is to create a new array for each supported
		// vertex attribute (st, normals, etc.) whose size is equal to the number of triangles
		// multiplied by 3, and then set the value of the vertex attribute at each vertex
		// of each triangle using the input array (normals[], st[], etc.)
		// [/comment]
		normals = std::unique_ptr<Vec3f[]>(new Vec3f[numTris * 3]);
		texCoords = std::unique_ptr<Vec2f[]>(new Vec2f[numTris * 3]);
		for (uint32_t i = 0, k = 0; i < nFaces; ++i) { // for each  face
			for (uint32_t j = 0; j < faceIndex[i] - 2; ++j) { // for each triangle in the face
				indices[l] = vertsIndex[k];
				indices[l + 1] = vertsIndex[k + j + 1];
				indices[l + 2] = vertsIndex[k + j + 2];
				normals[l] = normals[k];
				normals[l + 1] = normals[k + j + 1];
				normals[l + 2] = normals[k + j + 2];
				texCoords[l] = st[k];
				texCoords[l + 1] = st[k + j + 1];
				texCoords[l + 2] = st[k + j + 2];
				l += 3;
			}
			k += faceIndex[i];
		}
		// you can use move if the input geometry is already triangulated
		//N = std::move(normals); // transfer ownership
		//sts = std::move(st); // transfer ownership
	}

	// Test if ray intersects this triangle mesh
	bool Intersect(const Vec3f &orig, const Vec3f &dir, float &tNear, uint32_t &triIndex, Vec2f &uv) const
	{
#if MT_ALGO
		//uint32_t j = 0;
		//bool intersects = false;
		//for (uint32_t i = 0; i < numTris; ++i)
		//{
		//	const Vec3f &v0 = positions[indices[j]];
		//	const Vec3f &v1 = positions[indices[j + 1]];
		//	const Vec3f &v2 = positions[indices[j + 2]];
		//	float t = kInfinity, u, v;
		//	if (rayTriangleIntersect(orig, dir, v0, v1, v2, t, u, v) && t < tNear)
		//	{
		//		tNear = t;
		//		uv.x = u;
		//		uv.y = v;
		//		triIndex = i;
		//		intersects = true;
		//	}
		//	j += 3;
		//}
		//return intersects;
		uint32_t j = 0;
		bool isect = false;
		for (uint32_t i = 0; i < numTris; ++i) {
			const Vec3f &v0 = positions[indices[j]];
			const Vec3f &v1 = positions[indices[j + 1]];
			const Vec3f &v2 = positions[indices[j + 2]];
			float t = kInfinity, u, v;
			if (rayTriangleIntersect(orig, dir, v0, v1, v2, t, u, v) && t < tNear) {
				tNear = t;
				uv.x = u;
				uv.y = v;
				triIndex = i;
				isect = true;
			}
			j += 3;
		}

		return isect;
#else
		return false;
#endif
	}

	// Get surface properties
	void GetSurfaceProperties(
		const Vec3f &hitPoint,
		const Vec3f &viewDirection,
		const uint32_t &triIndex,
		const Vec2f &uv,
		Vec3f &hitNormal,
		Vec2f &hitTextureCoordinates) const
	{
		//// face normal
		//const Vec3f &v0 = positions[indices[triIndex * 3]];
		//const Vec3f &v1 = positions[indices[triIndex * 3 + 1]];
		//const Vec3f &v2 = positions[indices[triIndex * 3 + 2]];
		//hitNormal = (v1 - v0).CrossProduct(v2 - v0);
		//hitNormal.Normalize();

		//// texture coordinates
		//const Vec2f &st0 = texCoords[triIndex * 3];
		//const Vec2f &st1 = texCoords[triIndex * 3 + 1];
		//const Vec2f &st2 = texCoords[triIndex * 3 + 2];
		//hitTextureCoordinates = (1 - uv.x - uv.y) * st0 + uv.x * st1 + uv.y * st2;

		//// vertex normal
		//const Vec3f &n0 = normals[triIndex * 3];
		//const Vec3f &n1 = normals[triIndex * 3 + 1];
		//const Vec3f &n2 = normals[triIndex * 3 + 2];
		//hitNormal = (1 - uv.x - uv.y) * n0 + uv.x * n1 + uv.y * n2;

		// face normal
		const Vec3f &v0 = positions[indices[triIndex * 3]];
		const Vec3f &v1 = positions[indices[triIndex * 3 + 1]];
		const Vec3f &v2 = positions[indices[triIndex * 3 + 2]];
		hitNormal = (v1 - v0).CrossProduct(v2 - v0);
		hitNormal.Normalize();

		// texture coordinates
		const Vec2f &st0 = texCoords[triIndex * 3];
		const Vec2f &st1 = texCoords[triIndex * 3 + 1];
		const Vec2f &st2 = texCoords[triIndex * 3 + 2];
		hitTextureCoordinates = (1 - uv.x - uv.y) * st0 + uv.x * st1 + uv.y * st2;
	}
};
