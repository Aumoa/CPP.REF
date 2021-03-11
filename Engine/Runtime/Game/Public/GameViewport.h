// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "DirectX/DirectXMinimal.h"

class Scene;
class DirectXCompatibleRenderTarget;
class DirectXDeviceBundle;

class GAME_API GameViewport : virtual public Object
{
	GameViewport(const GameViewport&) = delete;
	GameViewport(GameViewport&&) = delete;

public:
	using Super = Object;

private:
	int32 resX;
	int32 resY;
	TRefPtr<DirectXCompatibleRenderTarget> compatibleTarget;

public:
	GameViewport(DirectXDeviceBundle* deviceBundle);
	~GameViewport() override;

	virtual void RenderScene(ID3D12GraphicsCommandList4* inCommandList, Scene* inScene) = 0;
	DirectXCompatibleRenderTarget* GetCompatibleRenderTarget() const;

	void SetResolution(int32 x, int32 y);

	vs_property_get_auto(int32, ResolutionX, resX);
	vs_property_get_auto(int32, ResolutionY, resY);

protected:
	virtual void SetViewportResolution_Internal(int32 x, int32 y);
};