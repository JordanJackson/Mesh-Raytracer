#pragma once

#include "MathHeader.h"
#include "TriangleMesh.h"

TriangleMesh* generatePolySphere(const Matrix4x4f &o2w, float radius, uint32_t divisions)
{
	// generate points
	uint32_t numVertices = (divisions - 1) * divisions + 2;
	std::unique_ptr<Vec3f[]> positions(new Vec3f[numVertices]);
	std::unique_ptr<Vec3f[]> normals(new Vec3f[numVertices]);
	std::unique_ptr<Vec2f[]> texCoords(new Vec2f[numVertices]);

	float u = -PI_2;
	float v = -PI;
	float du = PI / divisions;
	float dv = 2 * PI / divisions;

	// nice property of spheres -> position == normal
	// bottom point
	positions[0] = normals[0] = Vec3f(0, -radius, 0);
	uint32_t k = 1;
	for (uint32_t i = 0; i < divisions - 1; i++)
	{
		u += du;
		v = -PI;
		for (uint32_t j = 0; j < divisions; j++)
		{
			// position and normal
			float x = radius * cos(u) * cos(v);
			float y = radius * sin(u);
			float z = radius * cos(u) * sin(v);

			positions[k] = normals[k] = Vec3f(x, y, z);
			// texture coordinates
			texCoords[k].x = u / PI + 0.5;
			texCoords[k].y = v * 0.5 / PI + 0.5;
			v += dv, k++;
		}
	}
	// top point
	positions[k] = normals[k] = Vec3f(0, radius, 0);

	uint32_t numPolys = divisions * divisions;
	std::unique_ptr<uint32_t[]> faceIndex(new uint32_t[numPolys]);
	std::unique_ptr<uint32_t[]> vertexIndex(new uint32_t[(6 + (divisions - 1) * 4) * divisions]);

	// create connectivity list
	uint32_t vIdx = 1, numV = 0, l = 0;
	k = 0;
	for (uint32_t i = 0; i < divisions; i++)
	{
		for (uint32_t j = 0; j < divisions; j++)
		{
			if (i == 0)
			{
				faceIndex[k++] = 3;
				vertexIndex[l] = 0;
				vertexIndex[l + 1] = j + vIdx;
				vertexIndex[l + 2] = (j == (divisions - 1)) ? vIdx : j + vIdx + 1;
				l += 3;
			}
			else if (i == (divisions - 1))
			{
				faceIndex[k++] = 3;
				vertexIndex[l] = j + vIdx + 1 - divisions;
				vertexIndex[l + 1] = vIdx + 1;
				vertexIndex[l + 2] = (j == (divisions - 1)) ? vIdx + 1 - divisions : j + vIdx + 2 - divisions;
				l += 3;
			}
			else
			{
				faceIndex[k++] = 4;
				vertexIndex[l] = j + vIdx + 1 - divisions;
				vertexIndex[l + 1] = j + vIdx + 1;
				vertexIndex[l + 2] = (j == (divisions - 1)) ? vIdx + 1 : j + vIdx + 2;
				vertexIndex[l + 3] = (j == (divisions - 1)) ? vIdx + 1 - divisions : j + vIdx + 2 - divisions;
				l += 4;
			}
			numV++;
		}
		vIdx = numV;
	}

	return new TriangleMesh(o2w, numPolys, faceIndex, vertexIndex, positions, normals, texCoords);
}

TriangleMesh* loadPolyMeshFromFile(const Matrix4x4f &o2w, const char *file)
{
	std::ifstream ifs;
	try
	{
		ifs.open(file);
		if (ifs.fail()) throw;
		std::stringstream ss;
		ss << ifs.rdbuf();
		uint32_t numFaces;
		ss >> numFaces;
		std::unique_ptr<uint32_t[]> faceIndex(new uint32_t[numFaces]);
		uint32_t vertIndexArraySize = 0;
		// read face index array
		for (uint32_t i = 0; i < numFaces; ++i)
		{
			ss >> faceIndex[i];
			vertIndexArraySize += faceIndex[i];
		}
		std::unique_ptr<uint32_t[]> vertIndex(new uint32_t[vertIndexArraySize]);
		uint32_t vertArraySize = 0;
		// reading vertex index array
		for (uint32_t i = 0; i < vertIndexArraySize; ++i)
		{
			ss >> vertIndex[i];
			if (vertIndex[i] > vertArraySize)
				vertArraySize = vertIndex[i];
		}
		vertArraySize += 1;
		// reading vertices
		std::unique_ptr<Vec3f[]> positions(new Vec3f[vertArraySize]);
		for (uint32_t i = 0; i < vertArraySize; ++i)
		{
			ss >> positions[i].x >> positions[i].y >> positions[i].z;
		}
		// reading normals
		std::unique_ptr<Vec3f[]> normals(new Vec3f[vertIndexArraySize]);
		for (uint32_t i = 0; i < vertIndexArraySize; ++i)
		{
			ss >> normals[i].x >> normals[i].y >> normals[i].z;
		}
		// read tex coordinates
		std::unique_ptr<Vec2f[]> texCoords(new Vec2f[vertIndexArraySize]);
		for (uint32_t i = 0; i < vertIndexArraySize; ++i)
		{
			ss >> texCoords[i].x >> texCoords[i].y;
		}

		return new TriangleMesh(o2w, numFaces, faceIndex, vertIndex, positions, normals, texCoords);
	}
	catch (...)
	{
		ifs.close();
	}
	ifs.close();

	return nullptr;
}