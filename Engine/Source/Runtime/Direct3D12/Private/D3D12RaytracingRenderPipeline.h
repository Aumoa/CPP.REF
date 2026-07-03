// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/RenderPipeline.h"
#include "D3D12Common.h"
#include "D3D12RaytracingRenderPipeline.gen.h"

namespace Ayla
{
	class Shader;

	ACLASS()
	class D3D12RaytracingRenderPipeline : public RaytracingRenderPipeline
	{
		GENERATED_BODY()

	private:
		ComPtr<ID3D12RootSignature> m_GlobalRootSignature;
		ComPtr<ID3D12RootSignature> m_LocalRootSignature;
		ComPtr<ID3D12StateObject> m_StateObject;

	public:
		D3D12RaytracingRenderPipeline(ID3D12Device5* device, SharedPtr<Shader> shader);

		virtual void SetCameraBufferView(CommandBuffer* cmd, Buffer* buffer, size_t offset) override;

		ID3D12StateObject* GetPipelineStateObject() const noexcept { return m_StateObject.Get(); }
	};
}
