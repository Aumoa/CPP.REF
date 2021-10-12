// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PrimitiveSceneInfo.h"
#include "LightSceneInfo.h"
#include "SceneStructuredBuffer.h"
#include <queue>

interface IRHIDevice;
interface IRHIDeviceContext;
interface IRHIBuffer;
class SceneView;
class SceneRenderTarget;
class SceneStructuredBufferAllocator;

class SCENERENDER_API SScene : implements SObject
{
	GENERATED_BODY(SScene)

private:
	IRHIDevice* _Device = nullptr;

	std::queue<int64> _PrimitiveIds;
	std::queue<int64> _LightIds;

	std::vector<std::optional<PrimitiveSceneInfo>> _Primitives;
	std::vector<std::optional<LightSceneInfo>> _Lights;

	IRHIBuffer* _ViewBuffer = nullptr;
	std::vector<std::vector<uint8>> _ViewBufferSysMem;
	size_t _NumViewBuffers;
	uint64 _RequiredSize;
	std::vector<uint8> _ViewBufferCachedMemory;

public:
	SScene(IRHIDevice* InDevice);
	~SScene() override;

	int64 AddPrimitive(const PrimitiveSceneInfo& InPrimitive);
	bool RemovePrimitive(int64 InPrimitiveId);
	void UpdatePrimitive(int64 InPrimitiveId, const PrimitiveSceneInfo& InPrimitive);

	int64 AddLight(const LightSceneInfo& InLight);
	bool RemoveLight(int64 InLightId);
	void UpdateLight(int64 InPrimitiveId, const LightSceneInfo& InLight);

	inline const std::vector<std::optional<PrimitiveSceneInfo>>& GetPrimitives() const { return _Primitives; }
	inline const std::vector<std::optional<LightSceneInfo>>& GetLights() const { return _Lights; }

	void BeginScene();
	void EndScene();

	IRHIDevice* GetDevice();

	SceneStructuredBuffer QueryViewBuffer(size_t InRequiredSize);
	void ApplyViewBuffers(IRHIDeviceContext* InContext);
	uint64 GetActualGPUVirtualAddress(const SceneStructuredBuffer& InBuffer);
};