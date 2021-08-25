// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Assets/StaticMesh.h"
#include "LogGame.h"
#include "Scene/StaticMeshRenderData.h"

StaticMesh::StaticMesh(const std::filesystem::path& path, std::wstring_view name, StaticMeshRenderData* renderData)
	: Super(path)
	, _name(name)
	, _renderData(renderData)
{
	_renderData->SetOuter(this);
}