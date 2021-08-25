// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <filesystem>

class Asset;

class GAME_API SoftObjectPtrBase
{
protected:
	static Asset* InternalLoadObject(const std::filesystem::path& importPath);
	static void InternalUnloadObject(const std::filesystem::path& importPath);
};

template<class T>
class SoftObjectPtr : public SoftObjectPtrBase
{
private:
	std::filesystem::path _path;
	T* _loadedObject = nullptr;

public:
	SoftObjectPtr()
	{
	}

	SoftObjectPtr(const std::filesystem::path& importPath)
		: _path(importPath)
	{
	}

	SoftObjectPtr(const SoftObjectPtr& rhs)
		: _path(rhs._path)
		, _loadedObject(rhs._loadedObject)
	{
	}

	SoftObjectPtr(SoftObjectPtr&& rhs)
		: _path(std::move(rhs._path))
		, _loadedObject(rhs._loadedObject)
	{
		rhs._loadedObject = nullptr;
	}

	~SoftObjectPtr()
	{
		UnloadObject();
	}

	T* LoadObject()
	{
		if (_loadedObject == nullptr)
		{
			_loadedObject = dynamic_cast<T*>(InternalLoadObject(_path));
			_loadedObject->AddRef();
		}

		return _loadedObject;
	}

	void UnloadObject()
	{
		if (_loadedObject)
		{
			if (_loadedObject->ReleaseRef() == 0)
			{
				InternalUnloadObject(_path);
			}

			_loadedObject = nullptr;
		}
	}
};