// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class StaticMeshRenderData;

class StaticMesh : virtual public Object
{
public:
	using Super = Object;

private:
	std::wstring _name;

public:
	StaticMesh(std::wstring_view name);

	std::wstring GetName() const { return _name; }

protected:
	StaticMeshRenderData* _RenderData = nullptr;

public:
	StaticMeshRenderData* GetRenderData() const { return _RenderData; }
};