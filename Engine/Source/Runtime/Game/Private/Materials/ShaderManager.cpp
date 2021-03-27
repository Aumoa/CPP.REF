// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Materials/ShaderManager.h"

#include "Engine.h"
#include "Materials/Material.h"

ShaderManager::ShaderManager(Engine* engine) : Super()
{

}

ShaderManager::~ShaderManager()
{

}

void ShaderManager::ClearRef()
{
	referenceMap.clear();
}

void ShaderManager::AddMaterialRef(MaterialInterface* inMaterialRef)
{
	if (auto it = referenceMap.find(inMaterialRef); it != referenceMap.end())
	{
		it->second += 1;
	}
	else
	{
		referenceMap.emplace(inMaterialRef, 1);
	}
}

void ShaderManager::ResolveMaterials()
{
	for (auto& item : referenceMap)
	{
		item.first->ResolveDirtyState();
	}
}