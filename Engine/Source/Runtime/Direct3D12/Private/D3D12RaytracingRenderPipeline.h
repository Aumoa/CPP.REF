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
		ID3D12Device5* m_Device;
		ComPtr<ID3D12RootSignature> m_GlobalRootSignature;
		ComPtr<ID3D12RootSignature> m_LocalRootSignature;
		ComPtr<ID3D12StateObject> m_StateObject;
		ComPtr<ID3D12StateObjectProperties> m_StateObjectProperties;
		ComPtr<ID3D12Resource> m_TriangleVertexBuffer;
		ComPtr<ID3D12Resource> m_TriangleIndexBuffer;
		ComPtr<ID3D12Resource> m_BottomLevelAccelerationStructure;
		ComPtr<ID3D12Resource> m_TopLevelAccelerationStructure;
		ComPtr<ID3D12Resource> m_InstanceDescResource;
		ComPtr<ID3D12Resource> m_ScratchResource;
		ComPtr<ID3D12Resource> m_ShaderBindingTable;
		String m_RayGenerationExportName;
		String m_MissExportName;
		String m_HitGroupExportName = TEXT("DefaultHit");
		D3D12_RAYTRACING_GEOMETRY_DESC m_TriangleGeometry = {};
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC m_BottomLevelBuildDesc = {};
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC m_TopLevelBuildDesc = {};
		D3D12_DISPATCH_RAYS_DESC m_DispatchRaysDescTemplate = {};

	public:
		D3D12RaytracingRenderPipeline(ID3D12Device5* device, SharedPtr<Shader> shader);

		virtual void SetCameraBufferView(CommandBuffer* cmd, Buffer* buffer, size_t offset) override;

		ID3D12StateObject* GetPipelineStateObject() const noexcept { return m_StateObject.Get(); }
		ID3D12RootSignature* GetGlobalRootSignature() const noexcept { return m_GlobalRootSignature.Get(); }
		const D3D12_DISPATCH_RAYS_DESC& GetDispatchRaysDescTemplate() const noexcept { return m_DispatchRaysDescTemplate; }
		D3D12_GPU_VIRTUAL_ADDRESS GetTopLevelAccelerationStructureGPUVirtualAddress() const noexcept;
		void BuildAccelerationStructures(ID3D12GraphicsCommandList4* commandList);

	private:
		void CreateTestTriangleAccelerationStructures();
		void CreateShaderBindingTable();
		void CreateBuffer(uint64 size, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_FLAGS flags, D3D12_RESOURCE_STATES initialState, ComPtr<ID3D12Resource>* outResource) const;
		void UploadBuffer(ID3D12Resource* resource, std::span<const byte> data) const;
	};
}
