#include "Mesh.h"
#include "Triangle.h"
#include "OBJ_Loader.hpp"
#include <array>
#include "Model.h"

Model::Model(const std::string& filename)
{
    
	objl::Loader loader;
	loader.LoadFile(filename);
	float area = 0;

    for (size_t i = 0; i < loader.LoadedMeshes.size(); i++)
    {
        auto mesh = loader.LoadedMeshes[i];
        Mesh* pMesh = new Mesh();
        glm::vec3 min_vert = glm::vec3(std::numeric_limits<float>::infinity(),
            std::numeric_limits<float>::infinity(),
            std::numeric_limits<float>::infinity());
        glm::vec3 max_vert = glm::vec3(-std::numeric_limits<float>::infinity(),
            -std::numeric_limits<float>::infinity(),
            -std::numeric_limits<float>::infinity());

        for (int i = 0; i < mesh.Vertices.size(); i += 3) {
            Triangle* t = new Triangle();

            for (int j = 0; j < 3; j++) {
                t->setVertex(j, glm::vec4(mesh.Vertices[i + j].Position.X, mesh.Vertices[i + j].Position.Y, mesh.Vertices[i + j].Position.Z, 1.0));
                t->setNormal(j, glm::vec3(mesh.Vertices[i + j].Normal.X, mesh.Vertices[i + j].Normal.Y, mesh.Vertices[i + j].Normal.Z));
                t->setTexCoord(j, glm::vec2(mesh.Vertices[i + j].TextureCoordinate.X, mesh.Vertices[i + j].TextureCoordinate.Y));
                /*min_vert = glm::vec3(std::min(min_vert.x, vert.x),
                    std::min(min_vert.y, vert.y),
                    std::min(min_vert.z, vert.z));
                max_vert = glm::vec3(std::max(max_vert.x, vert.x),
                    std::max(max_vert.y, vert.y),
                    std::max(max_vert.z, vert.z));*/
            }

            pMesh->triangles.push_back(t);
        }

        //for (auto& tri : pMesh->triangles) {
        //    area += tri->area;
        //}
        pMesh->area = area;
        m_vecMesh.push_back(pMesh);
    }
}

Model::Model()
{
}
Model::~Model()
{
}