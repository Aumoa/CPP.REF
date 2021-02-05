// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "SceneVisibility.h"

interface IRHICommandList;

class PrimitiveSceneProxy;
class Scene;
class SceneVisibility;

struct MinimalViewInfo;

class GAME_API SceneRenderer : virtual public Object
{
public:
	using Super = Object;
	using This = SceneRenderer;

private:
	Scene* renderScene;
	std::vector<SceneVisibility> visibilities;

public:
	SceneRenderer(Scene* scene);
	~SceneRenderer() override;

	virtual void CalcVisibility();
	virtual void RenderScene(IRHICommandList* immediateCommandList) = 0;

	void AddViewInfo(MinimalViewInfo& inView);

	vs_property_get(Scene*, TargetScene);
	Scene* TargetScene_get() const;

protected:
	const std::vector<SceneVisibility>& GetSceneVisibilities() const;
};