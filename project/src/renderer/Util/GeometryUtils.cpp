#include "GeometryUtils.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

std::shared_ptr<Mesh> GeometryUtils::CreateCube(float size)
{
    float halfSize = size * 0.5f;

    std::vector<Vertex> vertices = {
        // Front face
        {{-halfSize, -halfSize,  halfSize}, {1,1,1,1}, {0, 0}, {0, 0, 1}},
        {{ halfSize, -halfSize,  halfSize}, {1,1,1,1}, {1, 0}, {0, 0, 1}},
        {{ halfSize,  halfSize,  halfSize}, {1,1,1,1}, {1, 1}, {0, 0, 1}},
        {{-halfSize,  halfSize,  halfSize}, {1,1,1,1}, {0, 1}, {0, 0, 1}},
        // Back face
        {{ halfSize, -halfSize, -halfSize}, {1,1,1,1}, {0, 0}, {0, 0, -1}},
        {{-halfSize, -halfSize, -halfSize}, {1,1,1,1}, {1, 0}, {0, 0, -1}},
        {{-halfSize,  halfSize, -halfSize}, {1,1,1,1}, {1, 1}, {0, 0, -1}},
        {{ halfSize,  halfSize, -halfSize}, {1,1,1,1}, {0, 1}, {0, 0, -1}},
        // Top face
        {{-halfSize,  halfSize,  halfSize}, {1,1,1,1}, {0, 0}, {0, 1, 0}},
        {{ halfSize,  halfSize,  halfSize}, {1,1,1,1}, {1, 0}, {0, 1, 0}},
        {{ halfSize,  halfSize, -halfSize}, {1,1,1,1}, {1, 1}, {0, 1, 0}},
        {{-halfSize,  halfSize, -halfSize}, {1,1,1,1}, {0, 1}, {0, 1, 0}},
        // Bottom face
        {{-halfSize, -halfSize, -halfSize}, {1,1,1,1}, {0, 0}, {0, -1, 0}},
        {{ halfSize, -halfSize, -halfSize}, {1,1,1,1}, {1, 0}, {0, -1, 0}},
        {{ halfSize, -halfSize,  halfSize}, {1,1,1,1}, {1, 1}, {0, -1, 0}},
        {{-halfSize, -halfSize,  halfSize}, {1,1,1,1}, {0, 1}, {0, -1, 0}},
        // Right face
        {{ halfSize, -halfSize,  halfSize}, {1,1,1,1}, {0, 0}, {1, 0, 0}},
        {{ halfSize, -halfSize, -halfSize}, {1,1,1,1}, {1, 0}, {1, 0, 0}},
        {{ halfSize,  halfSize, -halfSize}, {1,1,1,1}, {1, 1}, {1, 0, 0}},
        {{ halfSize,  halfSize,  halfSize}, {1,1,1,1}, {0, 1}, {1, 0, 0}},
        // Left face
        {{-halfSize, -halfSize, -halfSize}, {1,1,1,1}, {0, 0}, {-1, 0, 0}},
        {{-halfSize, -halfSize,  halfSize}, {1,1,1,1}, {1, 0}, {-1, 0, 0}},
        {{-halfSize,  halfSize,  halfSize}, {1,1,1,1}, {1, 1}, {-1, 0, 0}},
        {{-halfSize,  halfSize, -halfSize}, {1,1,1,1}, {0, 1}, {-1, 0, 0}}
    };

    std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0,       // Front
        4, 5, 6, 6, 7, 4,       // Back
        8, 9, 10, 10, 11, 8,    // Top
        12, 13, 14, 14, 15, 12, // Bottom
        16, 17, 18, 18, 19, 16, // Right
        20, 21, 22, 22, 23, 20  // Left
    };

    return std::make_shared<Mesh>(indices, vertices);
}

std::shared_ptr<Mesh> GeometryUtils::CreateSphere(float radius, int sectors, int stacks)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * glm::pi<float>() / sectors;
    float stackStep = glm::pi<float>() / stacks;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stacks; ++i)
    {
        stackAngle = glm::pi<float>() / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectors; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            
            Vertex vertex;
            vertex.pos = glm::vec3(x, y, z);
            vertex.color = glm::vec4(1.0f);

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            vertex.normal = glm::vec3(nx, ny, nz);

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectors;
            t = (float)i / stacks;
            vertex.texCoord = glm::vec2(s, t);

            vertices.push_back(vertex);
        }
    }

    // generate CCW index list of sphere triangles
    int k1, k2;
    for (int i = 0; i < stacks; ++i)
    {
        k1 = i * (sectors + 1);     // beginning of current stack
        k2 = k1 + sectors + 1;      // beginning of next stack

        for (int j = 0; j < sectors; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stacks - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    return std::make_shared<Mesh>(indices, vertices);
}

std::shared_ptr<Mesh> GeometryUtils::CreatePlane(float width, float height)
{
    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;

    std::vector<Vertex> vertices = {
        {{-halfWidth, 0.0f,  halfHeight}, {1,1,1,1}, {0, 0}, {0, 1, 0}},
        {{ halfWidth, 0.0f,  halfHeight}, {1,1,1,1}, {1, 0}, {0, 1, 0}},
        {{ halfWidth, 0.0f, -halfHeight}, {1,1,1,1}, {1, 1}, {0, 1, 0}},
        {{-halfWidth, 0.0f, -halfHeight}, {1,1,1,1}, {0, 1}, {0, 1, 0}}
    };

    std::vector<uint32_t> indices = {
        0, 1, 2,
        2, 3, 0
    };

    return std::make_shared<Mesh>(indices, vertices);
}

std::shared_ptr<Mesh> GeometryUtils::CreateCylinder(float radius, float height, int sectors)
{
     std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    float halfHeight = height * 0.5f;
    float sectorStep = 2 * glm::pi<float>() / sectors;

    // Generate side vertices
    for (int i = 0; i <= sectors; ++i) {
        float angle = i * sectorStep;
        float x = radius * cosf(angle);
        float z = radius * sinf(angle);
        float u = (float)i / sectors;

        // Top edge
        Vertex vTop;
        vTop.pos = glm::vec3(x, halfHeight, z);
        vTop.normal = glm::vec3(x / radius, 0.0f, z / radius); // Side normal
        vTop.texCoord = glm::vec2(u, 0.0f);
        vTop.color = glm::vec4(1.0f);
        vertices.push_back(vTop);

        // Bottom edge
        Vertex vBottom;
        vBottom.pos = glm::vec3(x, -halfHeight, z);
        vBottom.normal = glm::vec3(x / radius, 0.0f, z / radius); // Side normal
        vBottom.texCoord = glm::vec2(u, 1.0f);
        vBottom.color = glm::vec4(1.0f);
        vertices.push_back(vBottom);
    }

    // Side indices
    for (int i = 0; i < sectors; ++i) {
        int top1 = i * 2;
        int bottom1 = i * 2 + 1;
        int top2 = (i + 1) * 2;
        int bottom2 = (i + 1) * 2 + 1;

        indices.push_back(bottom1);
        indices.push_back(top2);
        indices.push_back(top1);
        
        indices.push_back(bottom1);
        indices.push_back(bottom2);
        indices.push_back(top2);
    }

    // Top cap
    int baseIndex = vertices.size();
    Vertex topCenter;
    topCenter.pos = glm::vec3(0.0f, halfHeight, 0.0f);
    topCenter.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    topCenter.texCoord = glm::vec2(0.5f, 0.5f);
    topCenter.color = glm::vec4(1.0f);
    vertices.push_back(topCenter);

    for (int i = 0; i <= sectors; ++i) {
        float angle = i * sectorStep;
        float x = radius * cosf(angle);
        float z = radius * sinf(angle);
        
        Vertex v;
        v.pos = glm::vec3(x, halfHeight, z);
        v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        v.texCoord = glm::vec2((cosf(angle) + 1.0f) * 0.5f, (sinf(angle) + 1.0f) * 0.5f);
        v.color = glm::vec4(1.0f);
        vertices.push_back(v);
    }

    for (int i = 0; i < sectors; ++i) {
        indices.push_back(baseIndex);
        indices.push_back(baseIndex + 1 + i);
        indices.push_back(baseIndex + 2 + i);
    }

    // Bottom cap
    baseIndex = vertices.size();
    Vertex bottomCenter;
    bottomCenter.pos = glm::vec3(0.0f, -halfHeight, 0.0f);
    bottomCenter.normal = glm::vec3(0.0f, -1.0f, 0.0f);
    bottomCenter.texCoord = glm::vec2(0.5f, 0.5f);
    bottomCenter.color = glm::vec4(1.0f);
    vertices.push_back(bottomCenter);

    for (int i = 0; i <= sectors; ++i) {
        float angle = i * sectorStep;
        float x = radius * cosf(angle);
        float z = radius * sinf(angle);

        Vertex v;
        v.pos = glm::vec3(x, -halfHeight, z);
        v.normal = glm::vec3(0.0f, -1.0f, 0.0f);
        v.texCoord = glm::vec2((cosf(angle) + 1.0f) * 0.5f, (sinf(angle) + 1.0f) * 0.5f);
        v.color = glm::vec4(1.0f);
        vertices.push_back(v);
    }

    for (int i = 0; i < sectors; ++i) {
        indices.push_back(baseIndex);
        indices.push_back(baseIndex + 2 + i);
        indices.push_back(baseIndex + 1 + i);
    }

    return std::make_shared<Mesh>(indices, vertices);
}
