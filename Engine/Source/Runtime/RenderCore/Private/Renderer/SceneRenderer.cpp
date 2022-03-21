// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Renderer/SceneRenderer.h"

GENERATE_BODY(SSceneRenderer);

SSceneRenderer::SSceneRenderer(IRHIDevice* device)
	: _device(device)
{
	_commandAllocator = _device->CreateCommandAllocator(ERHICommandListType::Direct);
	_commandBuffer = _device->CreateCommandList(_commandAllocator, ERHICommandListType::Direct, nullptr);
}

void SSceneRenderer::BeginDraw()
{
	_commandAllocator->Reset();
	_commandBuffer->Reset(_commandAllocator, nullptr);
}

void SSceneRenderer::EndDraw()
{
	_commandBuffer->Close();
}