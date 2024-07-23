#pragma once
#include "Object.h"
#include <iostream>
#include <glm/glm.hpp>

class Triangle;
class Mesh:public Object
{
public:
	std::unique_ptr<glm::vec3[]> vertices;
	uint32_t numTriangles;
	std::unique_ptr<uint32_t[]> vertexIndex;
	std::unique_ptr<glm::vec2[]> stCoordinates;

	std::vector<Triangle*> triangles;
	float area;

	Mesh();
	Mesh(const std::string& filename);
	~Mesh();
};

