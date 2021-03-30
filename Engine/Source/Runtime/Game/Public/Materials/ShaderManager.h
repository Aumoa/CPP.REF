// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class Engine;
class MaterialInterface;

class GAME_API ShaderManager : virtual public Object
{
public:
	using Super = Object;

private:
	std::map<MaterialInterface*, size_t> referenceMap;

public:
	ShaderManager(Engine* engine);
	~ShaderManager() override;

	void ClearRef();
	void AddMaterialRef(MaterialInterface* inMaterialRef);
	void ResolveMaterials();
};