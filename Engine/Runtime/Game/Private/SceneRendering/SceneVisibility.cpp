// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneVisibility.h"

#include "SceneRendering/Scene.h"
#include "SceneRendering/ShaderCameraConstant.h"
#include "Components/PrimitiveComponent.h"

using namespace std;

SceneVisibility::SceneVisibility(Scene* inScene) : Super()
	, myScene(inScene)
	, bDirty(true)
{
	shaderCameraConstants = NewObject<ShaderCameraConstantVector>();
}

SceneVisibility::~SceneVisibility()
{

}

void SceneVisibility::CalcVisibility()
{
	if (bDirty)
	{
		const auto& primitives = myScene->Primitives;

		visibilities.resize(primitives.size(), true);

		shaderCameraConstants->BeginUpdateConstant(myView);

		// PREVIEW IMPLEMENT. All primitives are visible.
		for (size_t i = 0; i < primitives.size(); ++i)
		{
			shaderCameraConstants->AddPrimitive(primitives[i]->GetSceneProxy());
		}

		shaderCameraConstants->EndUpdateConstant();

		bDirty = false;
	}
}

void SceneVisibility::UpdateView(const MinimalViewInfo& inView)
{
	myView = inView;
	bDirty = true;
}

const vector<bool>& SceneVisibility::PrimitiveVisibility_get() const
{
	return visibilities;
}

ShaderCameraConstantVector* SceneVisibility::ShaderCameraConstants_get() const
{
	return shaderCameraConstants.Get();
}