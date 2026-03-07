// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/RenderPipeline.h"
#include "D3D12Common.h"
#include "D3D12GeometryRenderPipeline.gen.h"

namespace Ayla
{
	class Shader;

	ACLASS()
	class D3D12GeometryRenderPipeline : public RenderPipeline
	{
		GENERATED_BODY()

	private:
		ComPtr<ID3D12RootSignature> m_RootSignature;
		ComPtr<ID3D12PipelineState> m_PipelineState;

	public:
		D3D12GeometryRenderPipeline(ID3D12Device5* device, std::vector<SharedPtr<Shader>> shaders);

		ID3D12RootSignature* GetRootSignature() const noexcept { return m_RootSignature.Get(); }
		ID3D12PipelineState* GetPipelineState() const noexcept { return m_PipelineState.Get(); }
	};
}