// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneRenderer.h"

using namespace std;

SceneRenderer::SceneRenderer(Scene* scene) : Super()
	, renderScene(scene)
{

}

SceneRenderer::~SceneRenderer()
{

}

Scene* SceneRenderer::GetScene() const
{
	return renderScene;
}