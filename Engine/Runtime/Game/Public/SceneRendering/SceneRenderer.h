// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "DirectX/DirectXMinimal.h"

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

	virtual void RenderScene(ID3D12GraphicsCommandList4* immediateCommandList) = 0;

	Scene* GetScene() const;
};