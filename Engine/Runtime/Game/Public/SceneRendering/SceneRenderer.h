// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHICommandList;
interface IRHIScene;

class GAME_API SceneRenderer : virtual public Object
{
public:
	using Super = Object;
	using This = SceneRenderer;

private:
	IRHIScene* renderScene;

public:
	SceneRenderer(IRHIScene* scene);
	~SceneRenderer() override;

	virtual void RenderScene(IRHICommandList* immediateCommandList) = 0;

	IRHIScene* GetScene() const;
};