// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "StaticMeshTest.h"
#include "Scene/StaticMeshRenderData.h"

using namespace std;

StaticMeshTest::StaticMeshTest(wstring_view name, RHIVertexFactory* vfactory) : Super(name)
{
	_RenderData = CreateSubobject<StaticMeshRenderData>(vfactory);
}