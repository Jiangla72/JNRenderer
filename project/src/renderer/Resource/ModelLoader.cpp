#include "ModelLoader.h"
#include "Util/StringUtil.h"
#include "Base/LogSystem.h"
#include "Render/Model.h"
#include "Render/Mesh.h"
#include "Render/Vertex.h"
#include "Render/Triangle.h"
#include "tiny_obj_loader.h"
#include <array>
#define ML_OBJ "obj"
#define ML_FBX "fbx"
#define ML_GLTF "gltf"
ModelLoader::ModelLoader()
{
}

ModelLoader::~ModelLoader()
{
}

bool ModelLoader::Load(const std::string& resPath, std::shared_ptr<IResource> pRes)
{
	bool bResult = false;
	auto count = pRes.use_count();
	std::shared_ptr<Model> model = std::dynamic_pointer_cast<Model>(pRes);

	count = pRes.use_count();
	count = model.use_count();

	if (model == nullptr)
	{
		return false;
	}
	std::string ext = StringUtil::GetFileExt(resPath);

	if (ext.compare(ML_OBJ) == 0)
	{
		count = model.use_count();
		bResult = _LoadOBJ(resPath, model);
		count = model.use_count();
	}
	else if (ext.compare(ML_FBX) == 0)
	{
		bResult = _LoadFBX(resPath, model);
	}
	else if (ext.compare(ML_GLTF) == 0)
	{
		bResult = _LoadGLTF(resPath, model);
	}
	else
		JNLOGERROR("��֧�ֵ�ģ���ļ���׺��{}",ext);
	
	return bResult;
}

bool ModelLoader::_LoadOBJ(const std::string& resPath, std::shared_ptr<Model> pRes)
{ 
	tinyobj::attrib_t                attrib;
	std::vector<tinyobj::shape_t>    shapes;
	std::vector<tinyobj::material_t> materials;
	std::string                      warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, resPath.c_str()))
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

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(indices, vertices);

		for (int i = 0; i < indices.size(); i += 3)
		{
			Triangle* t = new Triangle();

			for (int j = 0; j < 3; j++) {
				t->setVertex(j, glm::vec4(vertices[indices[i + j]].pos, 1.f));
				t->setNormal(j, glm::vec3(vertices[indices[i + j]].normal));
				t->setTexCoord(j, glm::vec2(vertices[indices[i + j]].texCoord));
			}

			mesh->triangles.push_back(t);
		}

		pRes->m_vecMesh.push_back(mesh);
	}
	return true;
}

bool ModelLoader::_LoadFBX(const std::string& resPath, std::shared_ptr<Model> pRes)
{
	return true;
}

bool ModelLoader::_LoadGLTF(const std::string& resPath, std::shared_ptr<Model> pRes)
{
	return true;
}
