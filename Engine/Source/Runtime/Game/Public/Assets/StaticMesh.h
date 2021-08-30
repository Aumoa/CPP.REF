// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset.h"

class RHIVertexFactory;
class StaticMeshRenderData;
struct RHIVertex;

class GAME_API StaticMesh : public Asset
{
	CLASS_BODY(StaticMesh)

private:
	std::wstring _name;
	StaticMeshRenderData* _renderData = nullptr;

public:
	StaticMesh(const std::filesystem::path& path, std::wstring_view name, StaticMeshRenderData* renderData);

	std::wstring GetName() const { return _name; }

public:
	StaticMeshRenderData* GetRenderData() const { return _renderData; }
};