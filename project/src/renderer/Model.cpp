#include "Mesh.h"
#include "Triangle.h"
//#include "OBJ_Loader.hpp"
#include "tiny_obj_loader.h"
#include <array>
#include "Model.h"
#include "Vertex.h"
Model::Model(const std::string& filename)
{
	load(filename);
}

Model::Model()
{
}
Model::~Model()
{
}

void Model::render()
{
	for each (auto mesh in m_vecMesh)
	{
		mesh->draw();
	}
}

void Model::load(const std::string& obj)
{
    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;
    std::string                      warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, obj.c_str()))
    {
        throw std::runtime_error(warn + err);
    }


    for (const auto& shape : shapes)
    {
        std::vector<Vertex>                  vertices;
        std::vector<uint32_t>                indices;
        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

		for (const auto& index : shape.mesh.indices)
		{
			Vertex vertex{};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2] };

 			if (index.normal_index >= 0)
				vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2] };

			if (index.texcoord_index >= 0)
				vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1] };

			vertex.color = { 1.0f, 1.0f, 1.0f, 1.f };

			if (uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.emplace_back(uniqueVertices[vertex]);
		}

		if (attrib.normals.empty())
			Mesh::generateNormals(vertices, indices);

		Mesh* mesh = new Mesh(indices, vertices);

		for (int i = 0; i < indices.size(); i += 3)
		{
			Triangle* t = new Triangle();

			for (int j = 0; j < 3; j++) {
				t->setVertex(j, glm::vec4(vertices[indices[i+j]].pos, 1.f));
				t->setNormal(j, glm::vec3(vertices[indices[i + j]].normal));
				t->setTexCoord(j, glm::vec2(vertices[indices[i + j]].texCoord));
			}

			mesh->triangles.push_back(t);
		}
		m_vecMesh.push_back(mesh);
    }
}
