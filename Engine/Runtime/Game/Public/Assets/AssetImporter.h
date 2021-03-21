// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class Engine;
class StaticMesh;

class GAME_API AssetImporter : virtual public Object
{
public:
	using Super = Object;

private:
	Engine* engine;

public:
	AssetImporter(Engine* engine);
	~AssetImporter();

	TRefPtr<StaticMesh> ImportStaticMesh(TRefPtr<String> path);
};