// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneRenderer.h"

using namespace std;

SceneRenderer::SceneRenderer(IRHIScene* scene) : Super()
	, renderScene(scene)
{

}

SceneRenderer::~SceneRenderer()
{

}

IRHIScene* SceneRenderer::GetScene() const
{
	return renderScene;
}