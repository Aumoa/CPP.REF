// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "LightSceneInfo.h"
#include "SceneStructuredBuffer.h"
#include <queue>

struct IRHIDevice;
struct IRHIDeviceContext;
struct IRHIBuffer;
class SPrimitiveComponent;
class PrimitiveSceneProxy;
class SceneView;
class SceneRenderTarget;
class SceneStructuredBufferAllocator;

class GAME_API SScene : implements SObject, implements IDisposable
{
	GENERATED_BODY(SScene)

private:
	SPROPERTY(Device)
	IRHIDevice* Device = nullptr;
	bool bDisposed = false;

	std::queue<int64> PrimitiveIds;
	SPROPERTY(PrimitiveComponents)
	std::vector<SPrimitiveComponent*> PrimitiveComponents;

	//SPROPERTY(ViewBuffer)
	//IRHIBuffer* ViewBuffer = nullptr;
	std::vector<std::vector<uint8>> ViewBufferSysMem;
	size_t NumViewBuffers = 0;
	uint64 RequiredSize = 0;
	std::vector<uint8> ViewBufferCachedMemory;

public:
	SScene(IRHIDevice* InDevice);
	~SScene() override;

	virtual void Dispose();

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