// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Rendering/RaytracingSceneRenderer.h"
#include "RHI/RHICommandList.h"
#include "RHI/RHIStructures.h"
#include "RHI/RHIDevice.h"
#include "RHI/RHIRootSignature.h"
#include "RHI/RHIShaderBindingTable.h"
#include "RHI/RHIRaytracingPipelineState.h"
#include "Rendering/SceneView.h"
#include "RTShaderLibrary.generated.h"
#include "RaytracingSceneRenderer.gen.cpp"

RaytracingSceneRenderer::RaytracingSceneRenderer(RHIDevice* InDevice)
{
	InitShaderPipeline(InDevice);
}

RaytracingSceneRenderer::~RaytracingSceneRenderer() noexcept
{
}

void RaytracingSceneRenderer::Render(SceneView* Scene, RHICommandList* CmdList)
{
	CmdList->SetComputeRootSignature(RootSignature.get());
	CmdList->SetPipelineState(PipelineState.get());

	RHIDispatchRaysDesc dispatchRays =
	{
		.RayGenerationShaderRecord = BindingTable->GetRayGenerationShaderRecord(PipelineState->GetShaderIdentifier(TEXT("RayGeneration"))),
		.MissShaderTable = BindingTable->GetMissShaderTable(),
		.HitGroupTable = BindingTable->GetHitGroupTable(),
		.CallableShaderTable = BindingTable->GetCallableShaderTable(),
		.Width = (uint32)1920,
		.Height = (uint32)1200,
		.Depth = 1
	};

	uint64 heapPtr = DescriptorTable->CopyFrom(0, RenderTarget.UAV.get(), 0, 1);
	CmdList->SetComputeRootDescriptorTable(0, heapPtr);
	CmdList->DispatchRays(dispatchRays);
}

void RaytracingSceneRenderer::InitShaderPipeline(RHIDevice* InDevice)
{
	RHIDescriptorRange Range =
	{
		.RangeType = ERHIDescriptorRangeType::UnorderedAccessView,
		.NumDescriptors = 1,
		.BaseShaderRegister = 0,
		.RegisterSpace = 0,
		.OffsetInDescriptorsFromTableStart = RHIDescriptorRange::DescriptorRangeOffsetAppend
	};

	RHIRootDescriptorTable Descriptor =
	{
		.NumDescriptorRanges = 1,
		.pDescriptorRanges = &Range
	};

	RHIRootParameter Parameter =
	{
		.ParameterType = ERHIRootParameterType::DescriptorTable,
		.DescriptorTable = Descriptor,
		.ShaderVisibility = ERHIShaderVisibility::All
	};

	RHIRootSignatureDesc RSDesc =
	{
		.NumParameters = 1,
		.pParameters = &Parameter,
		.Flags = ERHIRootSignatureFlags::DenyAllShaderRootAccess
	};

	RootSignature = InDevice->CreateRootSignature(RSDesc);

	std::shared_ptr ShaderBytecode = InDevice->CreateShaderBytecode(HLSL_RTShaderLibrary, SE_ARRAYSIZE(HLSL_RTShaderLibrary));

	RHIShaderLibraryExport ShaderExport =
	{
		.pShaderBytecode = ShaderBytecode.get(),
		.pGlobalRS = RootSignature.get(),
		.Exposes = { {.Name = TEXT("RayGeneration") } }
	};

	PipelineState = InDevice->CreateRaytracingPipelineState(ShaderExport);

	BindingTable = Device->CreateShaderBindingTable();
}