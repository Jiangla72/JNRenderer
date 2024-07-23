#pragma once
#include "Object.h"
#include <glm/glm.hpp>

class Triangle : public Object
{
public:
    glm::vec4 v[3]; /*the original coordinates of the triangle, v0, v1, v2 in counter clockwise order*/
                    /*Per vertex values*/
    glm::vec3 color[3]; //color at each vertex;
    glm::vec2 tex_coords[3]; //texture u,v
    glm::vec3 normal[3]; //normal vector for each vertex

    Triangle();

     glm::vec4 a() const { return v[0]; }
     glm::vec4 b() const { return v[1]; }
     glm::vec4 c() const { return v[2]; }

    void setVertex(int ind, glm::vec4 ver); /*set i-th vertex coordinates */
    void setNormal(int ind, glm::vec3 n); /*set i-th vertex normal vector*/
    void setColor(int ind, float r, float g, float b); /*set i-th vertex color*/

    void setNormals(glm::vec3* normals);
    void setColors(glm::vec3* colors);
    void setTexCoord(int ind, glm::vec2 uv); /*set i-th vertex texture coordinate*/
	
};

