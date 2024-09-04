#pragma once
#include "Object.h"
#include <iostream>
#include <glm/glm.hpp>

struct Vertex;
class Triangle;
class Mesh
{
public:
	std::unique_ptr<glm::vec3[]> vertices;
	uint32_t numTriangles;
	std::unique_ptr<uint32_t[]> vertexIndex;
	std::unique_ptr<glm::vec2[]> stCoordinates;
	std::vector<Triangle*> triangles;
	float area;

	uint32_t uIndices = -1;
private:
	unsigned int VBO, VAO, EBO;

public:
	Mesh();
	Mesh(const std::vector<uint32_t>& indices, const std::vector<Vertex>& vertices);
	~Mesh();

	static void generateNormals(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
	void draw();
};

