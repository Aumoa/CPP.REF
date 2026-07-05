// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12RaytracingRenderPipeline.h"
#include "D3D12Shader.h"
#include "RaytracePayload.h"
#include "RaytraceAttributes.h"
#include "Rendering/ShaderType.h"

#include <algorithm>
#include <cstring>

namespace Ayla
{
	namespace
	{
		uint64 AlignUp(uint64 value, uint64 alignment) noexcept
		{
			return alignment > 0 ? ((value + alignment - 1) / alignment) * alignment : value;
		}

		void ValidateRequiredShader(const Shader& shader, ShaderType shaderType, String shaderName)
		{
			if (shader.Has(shaderType) == false)
			{
				throw InvalidOperationException(String::Format(
					TEXT("Direct3D12 raytracing render pipeline requires a {} shader."),
					shaderName
				));
			}
		}

		struct RaytracingVertex
		{
			float m_X;
			float m_Y;
			float m_Z;
		};
	}

	D3D12RaytracingRenderPipeline::D3D12RaytracingRenderPipeline(ID3D12Device5* device, SharedPtr<Shader> shader)
		: m_Device(device)
	{
		if (m_Device == nullptr)
		{
			throw InvalidOperationException(TEXT("Direct3D12 raytracing render pipeline requires a device."));
		}

		Shader* shaderPtr = shader.Get();
		if (shaderPtr == nullptr)
		{
			throw InvalidOperationException(TEXT("Direct3D12 raytracing render pipeline requires a shader."));
		}

		ValidateRequiredShader(*shaderPtr, ShaderType::RayGeneration, TEXT("ray generation"));
		ValidateRequiredShader(*shaderPtr, ShaderType::Miss, TEXT("miss"));
		ValidateRequiredShader(*shaderPtr, ShaderType::ClosestHit, TEXT("closest hit"));

		m_RayGenerationExportName = shaderPtr->GetBytecode(ShaderType::RayGeneration).EntrypointName;
		m_MissExportName = shaderPtr->GetBytecode(ShaderType::Miss).EntrypointName;

		D3D12_DESCRIPTOR_RANGE outputTextureRange =
		{
			.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
			.NumDescriptors = 1,
			.BaseShaderRegister = 0,
			.RegisterSpace = 0,
			.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
		};

		std::array<D3D12_ROOT_PARAMETER, 2> rootParams =
		{
			D3D12_ROOT_PARAMETER
			{
				.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
				.DescriptorTable =
				{
					.NumDescriptorRanges = 1,
					.pDescriptorRanges = &outputTextureRange
				},
				.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL
			},
			D3D12_ROOT_PARAMETER
			{
				.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV,
				.Descriptor =
				{
					.ShaderRegister = 1,
					.RegisterSpace = 0
				},
				.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL
			}
		};

		D3D12_ROOT_SIGNATURE_DESC globalRootSigDesc =
		{
			.NumParameters = static_cast<UINT>(rootParams.size()),
			.pParameters = rootParams.data(),
			.NumStaticSamplers = 0,
			.pStaticSamplers = nullptr,
			.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE
		};

		ComPtr<ID3DBlob> globalRootSigBlob, errorBlob;
		HR(D3D12SerializeRootSignature(&globalRootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &globalRootSigBlob, &errorBlob));
		HR(m_Device->CreateRootSignature(0, globalRootSigBlob->GetBufferPointer(), globalRootSigBlob->GetBufferSize(), IID_PPV_ARGS(&m_GlobalRootSignature)));

		D3D12_ROOT_SIGNATURE_DESC localRootSigDesc =
		{
			.NumParameters = 0,
			.pParameters = nullptr,
			.NumStaticSamplers = 0,
			.pStaticSamplers = nullptr,
			.Flags = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE
		};

		ComPtr<ID3DBlob> localRootSigBlob;
		HR(D3D12SerializeRootSignature(&localRootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &localRootSigBlob, &errorBlob));
		HR(m_Device->CreateRootSignature(0, localRootSigBlob->GetBufferPointer(), localRootSigBlob->GetBufferSize(), IID_PPV_ARGS(&m_LocalRootSignature)));

		std::vector<const ShaderBytecode*> shaderBytecodeSources;
		shaderBytecodeSources.reserve(4);
		shaderBytecodeSources.emplace_back(&shaderPtr->GetBytecode(ShaderType::RayGeneration));
		shaderBytecodeSources.emplace_back(&shaderPtr->GetBytecode(ShaderType::ClosestHit));
		if (shaderPtr->Has(ShaderType::AnyHit))
		{
			shaderBytecodeSources.emplace_back(&shaderPtr->GetBytecode(ShaderType::AnyHit));
		}
		shaderBytecodeSources.emplace_back(&shaderPtr->GetBytecode(ShaderType::Miss));

		std::vector<String> entrypointNames;
		entrypointNames.reserve(shaderBytecodeSources.size());
		std::vector<D3D12_SHADER_BYTECODE> shaderBytecodes;
		shaderBytecodes.reserve(shaderBytecodeSources.size());
		std::vector<D3D12_DXIL_LIBRARY_DESC> dxilLibDesc;
		dxilLibDesc.reserve(shaderBytecodeSources.size());
		std::vector<D3D12_EXPORT_DESC> exports;
		exports.reserve(shaderBytecodeSources.size());
		std::vector<D3D12_STATE_SUBOBJECT> subobjects;
		subobjects.reserve(shaderBytecodeSources.size() + 5);
		for (size_t i = 0; i < shaderBytecodeSources.size(); ++i)
		{
			auto& shaderBytecodeSource = *shaderBytecodeSources[i];
			if (shaderBytecodeSource.Bytecode.empty())
			{
				throw InvalidOperationException(TEXT("Cannot create a Direct3D12 raytracing pipeline from empty shader bytecode."));
			}

			auto& entrypointName = entrypointNames.emplace_back(shaderBytecodeSource.EntrypointName);
			auto& shaderBytecode = shaderBytecodes.emplace_back(D3D12_SHADER_BYTECODE
			{
				.pShaderBytecode = shaderBytecodeSource.Bytecode.data(),
				.BytecodeLength = shaderBytecodeSource.Bytecode.size()
			});
			auto& exp = exports.emplace_back(D3D12_EXPORT_DESC
			{
				.Name = entrypointName.c_str(),
				.ExportToRename = nullptr,
				.Flags = D3D12_EXPORT_FLAG_NONE
			});
			auto& dxil = dxilLibDesc.emplace_back(D3D12_DXIL_LIBRARY_DESC
			{
				.DXILLibrary = shaderBytecode,
				.NumExports = 1,
				.pExports = &exp
			});
			subobjects.emplace_back(D3D12_STATE_SUBOBJECT
			{
				.Type = D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY,
				.pDesc = &dxil
			});
		}

		D3D12_HIT_GROUP_DESC hitGroupDesc = {};
		hitGroupDesc.HitGroupExport = m_HitGroupExportName.c_str();
		hitGroupDesc.Type = D3D12_HIT_GROUP_TYPE_TRIANGLES;
		hitGroupDesc.ClosestHitShaderImport = shaderPtr->GetBytecode(ShaderType::ClosestHit).EntrypointName.c_str();
		if (shaderPtr->Has(ShaderType::AnyHit))
		{
			hitGroupDesc.AnyHitShaderImport = shaderPtr->GetBytecode(ShaderType::AnyHit).EntrypointName.c_str();
		}

		D3D12_STATE_SUBOBJECT hitGroupSubobject = {};
		hitGroupSubobject.Type = D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
		hitGroupSubobject.pDesc = &hitGroupDesc;
		subobjects.emplace_back(hitGroupSubobject);

		D3D12_STATE_SUBOBJECT globalRootSigSubobject = {};
		globalRootSigSubobject.Type = D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
		globalRootSigSubobject.pDesc = m_GlobalRootSignature.GetAddressOf();
		subobjects.emplace_back(globalRootSigSubobject);

		D3D12_STATE_SUBOBJECT localRootSigSubobject = {};
		localRootSigSubobject.Type = D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
		localRootSigSubobject.pDesc = m_LocalRootSignature.GetAddressOf();
		subobjects.emplace_back(localRootSigSubobject);

		D3D12_RAYTRACING_SHADER_CONFIG shaderConfig = {};
		shaderConfig.MaxPayloadSizeInBytes = sizeof(RaytracePayload);
		shaderConfig.MaxAttributeSizeInBytes = sizeof(RaytraceAttributes);

		D3D12_STATE_SUBOBJECT shaderConfigSubobject = {};
		shaderConfigSubobject.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG;
		shaderConfigSubobject.pDesc = &shaderConfig;
		subobjects.emplace_back(shaderConfigSubobject);

		D3D12_RAYTRACING_PIPELINE_CONFIG pipelineConfig = {};
		pipelineConfig.MaxTraceRecursionDepth = 1;

		D3D12_STATE_SUBOBJECT pipelineConfigSubobject = {};
		pipelineConfigSubobject.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG;
		pipelineConfigSubobject.pDesc = &pipelineConfig;
		subobjects.emplace_back(pipelineConfigSubobject);

		D3D12_STATE_OBJECT_DESC stateObjectDesc =
		{
			.Type = D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE,
			.NumSubobjects = static_cast<UINT>(subobjects.size()),
			.pSubobjects = subobjects.data()
		};
		HR(m_Device->CreateStateObject(&stateObjectDesc, IID_PPV_ARGS(&m_StateObject)));
		HR(m_StateObject.As(&m_StateObjectProperties));

		CreateShaderBindingTable();
		CreateTestTriangleAccelerationStructures();
	}

	void D3D12RaytracingRenderPipeline::SetCameraBufferView(CommandBuffer* cmd, Buffer* buffer, size_t offset)
	{
		PLATFORM_UNREFERENCED_PARAMETER(cmd);
		PLATFORM_UNREFERENCED_PARAMETER(buffer);
		PLATFORM_UNREFERENCED_PARAMETER(offset);
	}

	D3D12_GPU_VIRTUAL_ADDRESS D3D12RaytracingRenderPipeline::GetTopLevelAccelerationStructureGPUVirtualAddress() const noexcept
	{
		return m_TopLevelAccelerationStructure ? m_TopLevelAccelerationStructure->GetGPUVirtualAddress() : 0;
	}

	void D3D12RaytracingRenderPipeline::BuildAccelerationStructures(ID3D12GraphicsCommandList4* commandList)
	{
		if (commandList == nullptr)
		{
			throw InvalidOperationException(TEXT("Direct3D12 acceleration structure build requires a command list."));
		}

		if (m_BottomLevelAccelerationStructure == nullptr || m_TopLevelAccelerationStructure == nullptr || m_ScratchResource == nullptr)
		{
			throw InvalidOperationException(TEXT("Direct3D12 acceleration structure resources are not allocated."));
		}

		commandList->BuildRaytracingAccelerationStructure(&m_BottomLevelBuildDesc, 0, nullptr);

		D3D12_RESOURCE_BARRIER bottomLevelBarrier =
		{
			.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV,
			.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
			.UAV =
			{
				.pResource = m_BottomLevelAccelerationStructure.Get()
			}
		};
		commandList->ResourceBarrier(1, &bottomLevelBarrier);

		commandList->BuildRaytracingAccelerationStructure(&m_TopLevelBuildDesc, 0, nullptr);

		D3D12_RESOURCE_BARRIER topLevelBarrier =
		{
			.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV,
			.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
			.UAV =
			{
				.pResource = m_TopLevelAccelerationStructure.Get()
			}
		};
		commandList->ResourceBarrier(1, &topLevelBarrier);
	}

	void D3D12RaytracingRenderPipeline::CreateTestTriangleAccelerationStructures()
	{
		static constexpr std::array<RaytracingVertex, 3> kVertices =
		{
			RaytracingVertex{ 0.0f, 1.0f, 0.0f },
			RaytracingVertex{ 1.0f, -1.0f, 0.0f },
			RaytracingVertex{ -1.0f, -1.0f, 0.0f },
		};

		static constexpr std::array<uint32, 3> kIndices = { 0, 1, 2 };

		CreateBuffer(sizeof(kVertices), D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, &m_TriangleVertexBuffer);
		UploadBuffer(m_TriangleVertexBuffer.Get(), std::span<const byte>(reinterpret_cast<const byte*>(kVertices.data()), sizeof(kVertices)));

		CreateBuffer(sizeof(kIndices), D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, &m_TriangleIndexBuffer);
		UploadBuffer(m_TriangleIndexBuffer.Get(), std::span<const byte>(reinterpret_cast<const byte*>(kIndices.data()), sizeof(kIndices)));

		m_TriangleGeometry =
		{
			.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES,
			.Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE,
			.Triangles =
			{
				.Transform3x4 = 0,
				.IndexFormat = DXGI_FORMAT_R32_UINT,
				.VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT,
				.IndexCount = static_cast<UINT>(kIndices.size()),
				.VertexCount = static_cast<UINT>(kVertices.size()),
				.IndexBuffer = m_TriangleIndexBuffer->GetGPUVirtualAddress(),
				.VertexBuffer =
				{
					.StartAddress = m_TriangleVertexBuffer->GetGPUVirtualAddress(),
					.StrideInBytes = sizeof(RaytracingVertex)
				}
			}
		};

		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS bottomLevelInputs =
		{
			.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL,
			.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE,
			.NumDescs = 1,
			.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY,
			.pGeometryDescs = &m_TriangleGeometry
		};

		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO bottomLevelSizeInfo = {};
		m_Device->GetRaytracingAccelerationStructurePrebuildInfo(&bottomLevelInputs, &bottomLevelSizeInfo);
		if (bottomLevelSizeInfo.ResultDataMaxSizeInBytes == 0)
		{
			throw InvalidOperationException(TEXT("Direct3D12 bottom-level acceleration structure prebuild info is invalid."));
		}

		CreateBuffer(
			AlignUp(bottomLevelSizeInfo.ResultDataMaxSizeInBytes, D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT),
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
			&m_BottomLevelAccelerationStructure
		);

		D3D12_RAYTRACING_INSTANCE_DESC instance = {};
		instance.Transform[0][0] = 1.0f;
		instance.Transform[1][1] = 1.0f;
		instance.Transform[2][2] = 1.0f;
		instance.InstanceID = 0;
		instance.InstanceMask = 0xFF;
		instance.InstanceContributionToHitGroupIndex = 0;
		instance.Flags = D3D12_RAYTRACING_INSTANCE_FLAG_TRIANGLE_CULL_DISABLE;
		instance.AccelerationStructure = m_BottomLevelAccelerationStructure->GetGPUVirtualAddress();

		CreateBuffer(sizeof(instance), D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, &m_InstanceDescResource);
		UploadBuffer(m_InstanceDescResource.Get(), std::span<const byte>(reinterpret_cast<const byte*>(&instance), sizeof(instance)));

		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS topLevelInputs =
		{
			.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL,
			.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE,
			.NumDescs = 1,
			.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY,
			.InstanceDescs = m_InstanceDescResource->GetGPUVirtualAddress()
		};

		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO topLevelSizeInfo = {};
		m_Device->GetRaytracingAccelerationStructurePrebuildInfo(&topLevelInputs, &topLevelSizeInfo);
		if (topLevelSizeInfo.ResultDataMaxSizeInBytes == 0)
		{
			throw InvalidOperationException(TEXT("Direct3D12 top-level acceleration structure prebuild info is invalid."));
		}

		CreateBuffer(
			AlignUp(topLevelSizeInfo.ResultDataMaxSizeInBytes, D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT),
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
			&m_TopLevelAccelerationStructure
		);

		const uint64 scratchSize = AlignUp(
			std::max(bottomLevelSizeInfo.ScratchDataSizeInBytes, topLevelSizeInfo.ScratchDataSizeInBytes),
			D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT
		);

		CreateBuffer(
			scratchSize,
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			&m_ScratchResource
		);

		const D3D12_GPU_VIRTUAL_ADDRESS scratchAddress = m_ScratchResource->GetGPUVirtualAddress();

		m_BottomLevelBuildDesc =
		{
			.DestAccelerationStructureData = m_BottomLevelAccelerationStructure->GetGPUVirtualAddress(),
			.Inputs = bottomLevelInputs,
			.SourceAccelerationStructureData = 0,
			.ScratchAccelerationStructureData = scratchAddress
		};

		m_TopLevelBuildDesc =
		{
			.DestAccelerationStructureData = m_TopLevelAccelerationStructure->GetGPUVirtualAddress(),
			.Inputs = topLevelInputs,
			.SourceAccelerationStructureData = 0,
			.ScratchAccelerationStructureData = scratchAddress
		};
	}

	void D3D12RaytracingRenderPipeline::CreateShaderBindingTable()
	{
		const uint64 shaderIdentifierSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
		const uint64 recordSize = AlignUp(shaderIdentifierSize, D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT);
		const uint64 tableAlignment = D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT;
		const uint64 rayGenerationOffset = 0;
		const uint64 missOffset = AlignUp(rayGenerationOffset + recordSize, tableAlignment);
		const uint64 hitGroupOffset = AlignUp(missOffset + recordSize, tableAlignment);
		const uint64 shaderBindingTableSize = AlignUp(hitGroupOffset + recordSize, tableAlignment);

		CreateBuffer(
			shaderBindingTableSize,
			D3D12_HEAP_TYPE_UPLOAD,
			D3D12_RESOURCE_FLAG_NONE,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			&m_ShaderBindingTable
		);

		std::vector<uint8> shaderBindingTableData(static_cast<size_t>(shaderBindingTableSize), 0);

		auto copyShaderIdentifier = [&](const String& exportName, uint64 destinationOffset)
		{
			void* shaderIdentifier = m_StateObjectProperties->GetShaderIdentifier(exportName.c_str());
			if (shaderIdentifier == nullptr)
			{
				throw InvalidOperationException(TEXT("Direct3D12 raytracing shader identifier was not found."));
			}

			std::memcpy(shaderBindingTableData.data() + destinationOffset, shaderIdentifier, shaderIdentifierSize);
		};

		copyShaderIdentifier(m_RayGenerationExportName, rayGenerationOffset);
		copyShaderIdentifier(m_MissExportName, missOffset);
		copyShaderIdentifier(m_HitGroupExportName, hitGroupOffset);

		UploadBuffer(m_ShaderBindingTable.Get(), std::span<const byte>(reinterpret_cast<const byte*>(shaderBindingTableData.data()), shaderBindingTableData.size()));

		const D3D12_GPU_VIRTUAL_ADDRESS shaderBindingTableAddress = m_ShaderBindingTable->GetGPUVirtualAddress();
		m_DispatchRaysDescTemplate =
		{
			.RayGenerationShaderRecord =
			{
				.StartAddress = shaderBindingTableAddress + rayGenerationOffset,
				.SizeInBytes = recordSize
			},
			.MissShaderTable =
			{
				.StartAddress = shaderBindingTableAddress + missOffset,
				.SizeInBytes = recordSize,
				.StrideInBytes = recordSize
			},
			.HitGroupTable =
			{
				.StartAddress = shaderBindingTableAddress + hitGroupOffset,
				.SizeInBytes = recordSize,
				.StrideInBytes = recordSize
			},
			.CallableShaderTable = {},
			.Width = 0,
			.Height = 0,
			.Depth = 1
		};
	}

	void D3D12RaytracingRenderPipeline::CreateBuffer(uint64 size, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_FLAGS flags, D3D12_RESOURCE_STATES initialState, ComPtr<ID3D12Resource>* outResource) const
	{
		if (size == 0)
		{
			throw InvalidOperationException(TEXT("Direct3D12 buffer size must be greater than zero."));
		}

		D3D12_HEAP_PROPERTIES heapProps =
		{
			.Type = heapType,
			.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN,
			.CreationNodeMask = 1,
			.VisibleNodeMask = 1
		};

		D3D12_RESOURCE_DESC resourceDesc =
		{
			.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
			.Alignment = 0,
			.Width = size,
			.Height = 1,
			.DepthOrArraySize = 1,
			.MipLevels = 1,
			.Format = DXGI_FORMAT_UNKNOWN,
			.SampleDesc =
			{
				.Count = 1,
				.Quality = 0
			},
			.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
			.Flags = flags
		};

		outResource->Reset();
		HR(m_Device->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			initialState,
			nullptr,
			IID_PPV_ARGS(outResource->GetAddressOf())
		));
	}

	void D3D12RaytracingRenderPipeline::UploadBuffer(ID3D12Resource* resource, std::span<const byte> data) const
	{
		if (resource == nullptr)
		{
			throw InvalidOperationException(TEXT("Direct3D12 upload requires a resource."));
		}

		void* mappedData = nullptr;
		D3D12_RANGE readRange = { 0, 0 };
		HR(resource->Map(0, &readRange, &mappedData));
		std::memcpy(mappedData, data.data(), data.size());
		resource->Unmap(0, nullptr);
	}
}
