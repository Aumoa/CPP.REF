// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHICommandList;

class Scene;

class GAME_API SceneRenderer : virtual public Object
{
public:
	using Super = Object;
	using This = SceneRenderer;

private:
	Scene* renderScene;

public:
	SceneRenderer(Scene* scene);
	~SceneRenderer() override;

	virtual void RenderScene(IRHICommandList* immediateCommandList) = 0;

	Scene* GetScene() const;
};