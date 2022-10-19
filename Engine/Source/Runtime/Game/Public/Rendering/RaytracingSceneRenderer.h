// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneRenderer.h"
#include "RaytracingSceneRenderer.gen.h"

class RHIDevice;
class RHIRootSignature;
class RHIRaytracingPipelineState;

SCLASS()
class GAME_API RaytracingSceneRenderer : public SceneRenderer
{
	GENERATED_BODY()

private:
	std::shared_ptr<RHIRootSignature> RootSignature;
	std::shared_ptr<RHIRaytracingPipelineState> PipelineState;
	std::shared_ptr<RHIShaderBindingTable> BindingTable;

public:
	RaytracingSceneRenderer(RHIDevice* InDevice);
	virtual ~RaytracingSceneRenderer() noexcept override;

	virtual void Render(SceneView* Scene, RHICommandList* CmdList) override;

private:
	void InitShaderPipeline(RHIDevice* InDevice);
};