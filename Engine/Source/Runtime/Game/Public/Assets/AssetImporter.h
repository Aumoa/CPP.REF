// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <filesystem>
#include "RenderMinimal.h"

class SGameEngine;
class SStaticMesh;
class SRHIVertexFactory;
class SAsset;

class GAME_API SAssetImporter : virtual public SObject
{
	GENERATED_BODY(SAssetImporter)

private:
	struct LoadedObjectData
	{
		SAsset* Ptr = nullptr;
	};

private:
	SGameEngine* _engine = nullptr;
	SRHIVertexFactory* _factory = nullptr;
	std::map<std::filesystem::path, LoadedObjectData> _assets;

public:
	SAssetImporter(SGameEngine* engine, SRHIVertexFactory* factory);

	void SearchContents();
	SAsset* LoadObject(const std::filesystem::path& importPath);
	void UnloadObject(const std::filesystem::path& importPath);

private:
	SAsset* LoadAssimpObject(const std::filesystem::path& importPath);
};