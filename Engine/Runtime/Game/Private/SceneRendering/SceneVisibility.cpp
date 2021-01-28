// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneVisibility.h"

#include "SceneRendering/Scene.h"

using namespace std;

SceneVisibility::SceneVisibility(Scene* inScene, MinimalViewInfo& inView) : Super()
	, myScene(inScene)
	, myView(inView)
{

}

SceneVisibility::SceneVisibility(SceneVisibility&& rh) noexcept : Super()
	, myScene(rh.myScene)
	, myView(rh.myView)
{

}

SceneVisibility::~SceneVisibility()
{

}

void SceneVisibility::CalcVisibility()
{
	// PREVIEW IMPLEMENT. All primitives are visible.
	visibilities.resize(myScene->Primitives.size(), true);
}

const vector<bool>& SceneVisibility::PrimitiveVisibility_get() const
{
	return visibilities;
}