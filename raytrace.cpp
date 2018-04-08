#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

#include "Geometry.h"
#include "MathHeader.h"
#include "Raytracer.h"

/* TODO LIST
	Refactor Origin & Dir into Ray class?
	Make Raytracer Class?
	Add basic triangle intersection test for speed comparison
	Add model loading (.geo)
	Look into other model types
	Acceleration Structure
	Multiple overlapping objects
	Sample from textures
*/

const int SEED = 24601;

float random_float(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

int main()
{
	srand(SEED);
	Options options;
	// Camera (View Matrix)
	//Matrix4x4f tmp = Matrix4x4f(0.707107, -0.331295, 0.624695, 0, 0, 0.883452, 0.468521, 0, -0.707107, -0.331295, 0.624695, 0, -1.63871, -5.747777, -40.400412, 1);
	Matrix4x4f tmp = Matrix4x4f(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, -100, 1);
	options.cameraToWorld = tmp.Inverse();
	options.fov = 50.0393;
	
	std::vector<std::unique_ptr<Object>> objects;
	options.outputName = "sphere";

	int numSpheres = 8;
	int numDivisions = 6;
	float positionVariance = 50.0f;
	float minRadius = 0.1f;
	float maxRadius = 10.0f;
	for (int i = 0; i < numSpheres; ++i)
	{
		Matrix4x4f modelMatrix = Matrix4x4f();

		Vec3f position = Vec3f(
			random_float(-positionVariance, positionVariance), 
			random_float(-positionVariance, positionVariance), 
			random_float(-positionVariance, positionVariance)
		);
		float radius = random_float(minRadius, maxRadius);
		modelMatrix.x[3][0] = position.x;
		modelMatrix.x[3][1] = position.y;
		modelMatrix.x[3][2] = position.z;
		//modelMatrix = modelMatrix.Translate(modelMatrix, position);
		objects.push_back(std::unique_ptr<Object>(generatePolySphere(modelMatrix, radius, numDivisions)));
	}

	// Cow
	//Matrix4x4f cowMat = Matrix4x4f();
	//TriangleMesh *cow = loadPolyMeshFromFile(cowMat, "cow.geo");

	//options.outputName = "cow";
	//objects.push_back(std::unique_ptr<Object>(cow));

	Render(options, objects, 0);

	return 0;
}