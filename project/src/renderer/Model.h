#pragma once
#include "Object.h"
#include <iostream>
#include <glm/glm.hpp>
class Mesh;
class Triangle;
class Model:public Object
{
public:
	std::vector<Mesh*> m_vecMesh;
public:
	Model();
	Model(const std::string& filename);
	~Model();
};

