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

// Math library is fine

int main()
{
	//Vec3f v(0, 1, 2);
	//std::cerr << v << std::endl;

	//Matrix4x4f a, b, c;
	//c = a * b;

	//Matrix4x4f d(0.707107, 0, -0.707107, 0, -0.331295, 0.883452, -0.331295, 0, 0.624695, 0.468521, 0.624695, 0, 4.000574, 3.00043, 4.000574, 1);
	//std::cerr << d << std::endl;
	//d.Invert();
	//std::cerr << d << std::endl;
	Options options;
	Matrix4x4f tmp = Matrix4x4f(0.707107, -0.331295, 0.624695, 0, 0, 0.883452, 0.468521, 0, -0.707107, -0.331295, 0.624695, 0, -1.63871, -5.747777, -40.400412, 1);
	options.cameraToWorld = tmp.Inverse();
	options.fov = 50.0393;

	//auto pos = std::unique_ptr<Vec3f[]>(new Vec3f[10]);

	// Sphere
	TriangleMesh *sphere = generatePolySphere(2, 5);

	// Cow
	TriangleMesh *cow = loadPolyMeshFromFile("cow.geo");

	std::vector<std::unique_ptr<Object>> objects;
	objects.push_back(std::unique_ptr<Object>(sphere));
	Render(options, objects, 0);


	//std::cin.get();

	return 0;
}