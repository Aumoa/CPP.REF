// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "SceneRenderer.h"

#include "DirectX/DirectXMinimal.h"

class DirectXShaderResourceView;
class DirectXDescriptorAllocator;
class DirectXShaderBindingTable;
class CachedShaderLibrary;
class SceneVisibility;

class GAME_API RaytracingSceneRenderer : public SceneRenderer
{
public:
	using Super = SceneRenderer;

	struct ShaderGlobalInputs
	{
		DirectXShaderResourceView* ColorOutput;
	};

private:
	CachedShaderLibrary* shaderLibrary;
	ShaderGlobalInputs globalInputs;

public:
	RaytracingSceneRenderer(Scene* inScene);
	~RaytracingSceneRenderer();

	virtual void RenderScene(ID3D12GraphicsCommandList4* inCommandList, GameViewport* inViewport);

	void InitGlobalInputs(const ShaderGlobalInputs& globalInputs);
};