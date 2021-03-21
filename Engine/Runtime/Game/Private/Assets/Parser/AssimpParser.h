// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "ParserBase.h"

namespace Assimp
{
	class Importer;
}

class Engine;
class Material;
struct aiScene;

class AssimpParser : public ParserBase
{
public:
	using Super = Object;

private:
	Engine* engine;
	std::unique_ptr<Assimp::Importer> myImporter;
	const aiScene* myScene;

	std::vector<TRefPtr<Material>> materials;
	TRefPtr<StaticMesh> staticMesh;

public:
	AssimpParser(Engine* engine);
	~AssimpParser() override;

	bool TryParse(TRefPtr<String> filepath) override;

	bool IsStaticMesh() const override;
	TRefPtr<StaticMesh> GetStaticMesh() const override;

private:
	bool ProcessMaterials();
	bool ProcessStaticMeshSubsets();
};