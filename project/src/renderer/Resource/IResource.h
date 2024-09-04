#pragma once
#include <string>

enum ResourceType
{
	eModel,
	eTexture2D,
	eShader,
};
class IResource
{
public:
	IResource() {};
	virtual ~IResource() {};
public:
	virtual void Init() = 0;

	virtual void Release() = 0;
	virtual const std::string& GetName() const = 0;
	virtual uint32_t GetSize() const = 0;
	static  ResourceType GetType() {};
	//virtual AddRefCount() const = 0;
};

//class Resource: IResource
//{
//public:
//	virtual void Init(const std::string& resFile) = 0;
//
//	virtual void Release() = 0;
//	virtual const std::string& GetName() const = 0;
//	virtual uint32_t GetSize() const = 0;
//
//	//virtual AddRefCount() const = 0;
//};