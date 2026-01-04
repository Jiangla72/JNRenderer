#pragma once
#include <vector>
#include <memory>
#include "Render/Mesh.h"
#include "Render/Vertex.h"

class GeometryUtils
{
public:
    static std::shared_ptr<Mesh> CreateCube(float size = 1.0f);
    static std::shared_ptr<Mesh> CreateSphere(float radius = 0.5f, int sectors = 32, int stacks = 16);
    static std::shared_ptr<Mesh> CreatePlane(float width = 10.0f, float height = 10.0f);
    static std::shared_ptr<Mesh> CreateCylinder(float radius = 0.5f, float height = 1.0f, int sectors = 32);

private:
    static void CalculateTangents(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
};
