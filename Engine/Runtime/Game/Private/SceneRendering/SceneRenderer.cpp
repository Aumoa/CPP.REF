// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneRenderer.h"

#include "SceneRendering/Scene.h"
#include "Shaders/ShaderCameraConstant.h"

using namespace std;

SceneRenderer::SceneRenderer(Scene* scene) : Super()
	, renderScene(scene)
{

}

SceneRenderer::~SceneRenderer()
{

}

void SceneRenderer::CalcVisibility()
{
	renderScene->ShaderCameraConstants->BeginUpdateConstant();

	for (auto& inst : visibilities)
	{
		inst.CalcVisibility();
	}

	renderScene->ShaderCameraConstants->EndUpdateConstant();
}

void SceneRenderer::AddViewInfo(MinimalViewInfo& inView)
{
	visibilities.emplace_back(renderScene, inView);
}

Scene* SceneRenderer::TargetScene_get() const
{
	return renderScene;
}

const vector<SceneVisibility>& SceneRenderer::GetSceneVisibilities() const
{
	return visibilities;
}