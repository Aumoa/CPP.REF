// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class RHIVertexFactory;
class StaticMeshRenderData;
struct RHIVertex;

class StaticMesh : virtual public Object
{
public:
	using Super = Object;

private:
	std::wstring _name;
	StaticMeshRenderData* _renderData = nullptr;

public:
	StaticMesh(std::wstring_view name, StaticMeshRenderData* renderData);

	std::wstring GetName() const { return _name; }

public:
	StaticMeshRenderData* GetRenderData() const { return _renderData; }
};