#include "Mesh.h"
#include "Triangle.h"
#include"Vertex.h"
#include <array>

#include<glad/glad.h>
#include<gl/GL.h>
#include <GLFW/glfw3.h>
Mesh::Mesh()
{
}

Mesh::Mesh(const std::vector<uint32_t>& indices, const std::vector<Vertex>& vertices)
{
	uIndices = indices.size();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), (&vertices[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE,  12 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
}

void Mesh::generateNormals(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	std::vector<glm::vec3> normals(vertices.size());

	if (!indices.empty())
	{
		for (uint32_t i = 0; i < indices.size(); i += 3)
		{
			const auto a = indices[i];
			const auto b = indices[i + 1];
			const auto c = indices[i + 2];
			const auto normal = glm::cross((vertices[b].pos - vertices[a].pos), (vertices[c].pos - vertices[a].pos));
			normals[a] += normal;
			normals[b] += normal;
			normals[c] += normal;
		}
	}
	else
	{
		for (uint32_t i = 0; i < vertices.size(); i += 3)
		{
			auto& a = vertices[i];
			auto& b = vertices[i + 1];
			auto& c = vertices[i + 2];
			const auto normal = glm::cross(b.pos - a.pos, c.pos - a.pos);
			normals[i] = normal;
			normals[i + 1] = normal;
			normals[i + 2] = normal;
		}
	}
	for (uint32_t i = 0; i < normals.size(); ++i)
	{
		vertices[i].normal = glm::normalize(normals[i]);
	}
}

void Mesh::draw()
{
	if (uIndices != -1)
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, uIndices, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}
}
