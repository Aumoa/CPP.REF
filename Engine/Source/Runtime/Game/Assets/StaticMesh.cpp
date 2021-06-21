// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "StaticMesh.h"
#include "LogGame.h"
#include "Scene/StaticMeshRenderData.h"

using namespace std;

StaticMesh::StaticMesh(wstring_view name, StaticMeshRenderData* renderData) : Super()
	, _name(name)
	, _renderData(renderData)
{
	_renderData->SetOuter(this);
}