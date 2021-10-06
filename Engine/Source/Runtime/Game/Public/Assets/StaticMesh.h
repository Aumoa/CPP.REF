// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset.h"

class SStaticMeshRenderData;
struct RHIVertex;

class GAME_API SStaticMesh : public SAsset
{
	GENERATED_BODY(SStaticMesh)

private:
	std::wstring _name;
	SStaticMeshRenderData* _renderData = nullptr;

public:
	SStaticMesh(const std::filesystem::path& path, std::wstring_view name, SStaticMeshRenderData* renderData);

	std::wstring GetName() { return _name; }

public:
	SStaticMeshRenderData* GetRenderData() { return _renderData; }
};