// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class StaticMesh;
class Material;
class Engine;
struct LogCategoryBase;

class GAME_API AssetManager : virtual public Object
{
public:
	using Super = Object;
	using This = AssetManager;

private:
	Engine* engine;
	std::map<TRefPtr<String>, TRefPtr<StaticMesh>> staticMeshes;
	std::map<TRefPtr<String>, TRefPtr<Material>> materials;
	TRefPtr<Material> defaultMaterial;

public:
	AssetManager(Engine* engine);
	~AssetManager() override;

	void Import(TRefPtr<String> key, TRefPtr<StaticMesh> imported);
	StaticMesh* LoadStaticMesh(TRefPtr<String> key) const;

	void Import(TRefPtr<String> key, TRefPtr<Material> imported);
	Material* LoadMaterial(TRefPtr<String> key) const;

private:
	void InitEngineAssets();
};