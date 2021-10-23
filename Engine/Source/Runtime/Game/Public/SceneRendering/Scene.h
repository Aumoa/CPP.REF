// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "LightSceneInfo.h"
#include "SceneStructuredBuffer.h"
#include <queue>

interface IRHIDevice;
interface IRHIDeviceContext;
interface IRHIBuffer;
class SPrimitiveComponent;
class PrimitiveSceneProxy;
class SceneView;
class SceneRenderTarget;
class SceneStructuredBufferAllocator;

class GAME_API SScene : implements SObject
{
	GENERATED_BODY(SScene)

private:
	IRHIDevice* _Device = nullptr;

	std::queue<int64> _PrimitiveIds;
	std::vector<SPrimitiveComponent*> _PrimitiveComponents;

	IRHIBuffer* _ViewBuffer = nullptr;
	std::vector<std::vector<uint8>> _ViewBufferSysMem;
	size_t _NumViewBuffers;
	uint64 _RequiredSize;
	std::vector<uint8> _ViewBufferCachedMemory;

public:
	SScene(IRHIDevice* InDevice);
	~SScene() override;

	bool AddPrimitive(SPrimitiveComponent* InPrimitive);
	bool RemovePrimitive(SPrimitiveComponent* InPrimitive);

	void BeginScene();
	void EndScene();

	IRHIDevice* GetDevice();

	SceneStructuredBuffer QueryViewBuffer(size_t InRequiredSize);
	void ApplyViewBuffers(IRHIDeviceContext* InContext);
	uint64 GetActualGPUVirtualAddress(const SceneStructuredBuffer& InBuffer);

private:
	std::vector<PrimitiveSceneProxy*> Primitives;

private:
	void AddSceneProxy_RenderThread(PrimitiveSceneProxy* InPrimitive);
	void RemoveSceneProxy_RenderThread(int64 InPrimitiveId);

public:
	const std::vector<PrimitiveSceneProxy*>& GetPrimitives_RenderThread();
};