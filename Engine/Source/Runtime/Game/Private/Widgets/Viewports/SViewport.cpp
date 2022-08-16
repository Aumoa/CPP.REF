// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/Viewports/SViewport.h"
#include "Widgets/Viewports/SWindow.h"
#include "Widgets/Drawing/PaintArgs.h"
#include "RHI/RHICommandList.h"
#include "RHI/RHIRenderThread.h"
#include "RHI/RHIStructures.h"
#include "RTShaderLibrary.generated.h"

constexpr LogCategory LogViewport = LogCategory(TEXT("LogViewport"));

SViewport::SViewport()
	: Super()
{
}

void SViewport::Tick(const Geometry& AllottedGeometry, const TimeSpan& deltaTime)
{
	Super::Tick(AllottedGeometry, deltaTime);
	TryResizeViewport(AllottedGeometry);
	ComposeRaytracingShaders();
}

int32 SViewport::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled)
{
	return InLayer;
}

void SViewport::PresentViewport(RHICommandList* commandList)
{
	commandList->SetComputeRootSignature(_rootSignature.get());
	commandList->SetPipelineState(_pipelineState.get());
}

DEFINE_SLATE_CONSTRUCTOR(SViewport, attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(attr);

	_device = attr._Window->GetDevice();
}

void SViewport::TryResizeViewport(const Geometry& allottedGeometry)
{
	Vector2N size = Vector<>::Cast<int32>(allottedGeometry.GetLocalSize());
	if (_renderTarget.Size != size)
	{
		EnqueueRenderThreadWork([ref = std::move(_renderTarget), size]() mutable
		{
			Log::Info(LogViewport, TEXT("Viewport resized from [{0}] to [{1}]."), ref.Size, size);
		});

		_renderTarget = RHIRaytracingRenderTarget::Create(_device, size);
	}
}

void SViewport::ComposeRaytracingShaders()
{
	if (!_rootSignature)
	{
		RHIDescriptorRange range =
		{
			.RangeType = ERHIDescriptorRangeType::UnorderedAccessView,
			.NumDescriptors = 1,
			.BaseShaderRegister = 0,
			.RegisterSpace = 0,
			.OffsetInDescriptorsFromTableStart = RHIDescriptorRange::DescriptorRangeOffsetAppend
		};

		RHIRootDescriptorTable descriptor =
		{
			.NumDescriptorRanges = 1,
			.pDescriptorRanges = &range
		};

		RHIRootParameter parameter =
		{
			.ParameterType = ERHIRootParameterType::DescriptorTable,
			.DescriptorTable = descriptor,
			.ShaderVisibility = ERHIShaderVisibility::All
		};

		RHIRootSignatureDesc rsdesc =
		{
			.NumParameters = 1,
			.pParameters = &parameter,
			.Flags = ERHIRootSignatureFlags::DenyAllShaderRootAccess
		};

		_rootSignature = _device->CreateRootSignature(rsdesc);
	}

	if (!_pipelineState)
	{
		std::shared_ptr shaderBytecode = _device->CreateShaderBytecode(HLSL_RTShaderLibrary, SE_ARRAYSIZE(HLSL_RTShaderLibrary));

		RHIShaderLibraryExport shaderExport =
		{
			.pShaderBytecode = shaderBytecode.get(),
			.pGlobalRS = _rootSignature.get(),
			.Exposes = { { .Name = TEXT("RayGeneration") } }
		};

		_pipelineState = _device->CreateRaytracingPipelineState(shaderExport);
	}
}