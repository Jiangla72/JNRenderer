#pragma once
#include "Base/core.h"
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Resource/IResource.h"
enum JNFormat
{
	JNF_RGB,
	JNF_RGBA,
};
struct TextureDesc
{
	uint16_t width = 0;
	uint16_t height = 0;
	JNFormat format = JNF_RGB;

};


class JNAPI Texture:public IResource
{
private:
	int width;
	int height;
	int nrChannels;
	unsigned int texture;

public:
	Texture(uint16_t width, uint16_t height, JNFormat nrChannels);
	Texture(const std::string& texturePath);
	Texture();
	~Texture();
	void Init(const std::string& cPath);

	unsigned int GetHandle() { return texture; };
	int ConvertEnmColorChanelToGLFormat(JNFormat enmType);
	bool BindTexture(uint16_t uLocation);
	bool UnBindTexture(uint16_t uLocation);

	// Í¨¹ý IResource ¼Ì³Ð
	void Init();
	void Release();
	const std::string& GetName() const;
	uint32_t GetSize() const;
	static  ResourceType GetType() {
		return eTexture2D;
	};

};

namespace TextureHelper {
	 std::shared_ptr<Texture> CreateTexture(const TextureDesc& sDesc);
	 std::shared_ptr<Texture> CreateTextureFromFile(const std::string& strFile);

};