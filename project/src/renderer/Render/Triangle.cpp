#include "Triangle.h"

Triangle::Triangle() {
    v[0] = { 0, 0, 0, 1 };
    v[1] ={0, 0, 0, 1};
    v[2] = { 0, 0, 0, 1 };

    color[0] ={0.0, 0.0, 0.0};
    color[1] ={0.0, 0.0, 0.0};
    color[2] = { 0.0, 0.0, 0.0 };

    tex_coords[0] ={ 0.0, 0.0};
    tex_coords[1] ={ 0.0, 0.0};
    tex_coords[2] = { 0.0, 0.0 };
}

void Triangle::setVertex(int ind, glm::vec4 ver) {
    v[ind] = ver;
}
void Triangle::setNormal(int ind, glm::vec3 n) {
    normal[ind] = n;
}
void Triangle::setColor(int ind, float r, float g, float b) {
    if ((r < 0.0) || (r > 255.) ||
        (g < 0.0) || (g > 255.) ||
        (b < 0.0) || (b > 255.)) {
        fprintf(stderr, "ERROR! Invalid color values");
        fflush(stderr);
        exit(-1);
    }

    color[ind] = glm::vec3((float)r / 255., (float)g / 255., (float)b / 255.);
    return;
}
void Triangle::setTexCoord(int ind, glm::vec2 uv) {
    tex_coords[ind] = uv;
}

void Triangle::setNormals(glm::vec3* normals)
{
    normal[0] = normals[0];
    normal[1] = normals[1];
    normal[2] = normals[2];
}

void Triangle::setColors(glm::vec3* colors)
{
    auto first_color = colors[0];
    setColor(0, colors[0][0], colors[0][1], colors[0][2]);
    setColor(1, colors[1][0], colors[1][1], colors[1][2]);
    setColor(2, colors[2][0], colors[2][1], colors[2][2]);
}
