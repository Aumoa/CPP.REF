// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class StaticMesh;
class Material;
struct LogCategoryBase;

class GAME_API AssetManager : virtual public Object
{
public:
	using Super = Object;
	using This = AssetManager;

private:
	static LogCategoryBase LogAssets;

	std::map<TRefPtr<String>, TRefPtr<StaticMesh>> staticMeshes;
	std::map<TRefPtr<String>, TRefPtr<Material>> materials;

public:
	AssetManager();
	~AssetManager() override;

	void Import(TRefPtr<String> key, TRefPtr<StaticMesh> imported);
	StaticMesh* LoadStaticMesh(TRefPtr<String> key) const;

	void Import(TRefPtr<String> key, TRefPtr<Material> imported);
	Material* LoadMaterial(TRefPtr<String> key) const;
};