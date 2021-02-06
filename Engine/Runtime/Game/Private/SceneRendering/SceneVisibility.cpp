// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneVisibility.h"

#include "SceneRendering/Scene.h"
#include "SceneRendering/ShaderCameraConstant.h"
#include "Components/PrimitiveComponent.h"

using namespace std;

SceneVisibility::SceneVisibility(Scene* inScene, MinimalViewInfo& inView) : Super()
	, myScene(inScene)
	, myView(inView)
{

}

SceneVisibility::SceneVisibility(const SceneVisibility& rh) : Super()
	, myScene(rh.myScene)
	, myView(rh.myView)
	, visibilities(rh.visibilities)
{

}

SceneVisibility::SceneVisibility(SceneVisibility&& rh) noexcept : Super()
	, myScene(rh.myScene)
	, myView(rh.myView)
	, visibilities(move(rh.visibilities))
{

}

SceneVisibility::~SceneVisibility()
{

}

void SceneVisibility::CalcVisibility()
{
	const auto& primitives = myScene->Primitives;

	// PREVIEW IMPLEMENT. All primitives are visible.
	visibilities.resize(primitives.size(), true);

	ShaderCameraConstantVector* cbv = myScene->ShaderCameraConstants;
	for (size_t i = 0; i < primitives.size(); ++i)
	{
		cbv->AddPrimitive(myView, primitives[i]->GetSceneProxy());
	}
}

const vector<bool>& SceneVisibility::PrimitiveVisibility_get() const
{
	return visibilities;
}