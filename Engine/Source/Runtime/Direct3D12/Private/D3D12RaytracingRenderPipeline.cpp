// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12RaytracingRenderPipeline.h"
#include "D3D12Shader.h"
#include "RaytracePayload.h"
#include "RaytraceAttributes.h"

namespace Ayla
{
	D3D12RaytracingRenderPipeline::D3D12RaytracingRenderPipeline(ID3D12Device5* device, std::vector<SharedPtr<Shader>> shaders)
	{
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
					.ShaderRegister = 0,
					.RegisterSpace = 0
				},
				.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL
			}
		};

		D3D12_ROOT_SIGNATURE_DESC globalRootSigDesc =
		{
			static_cast<UINT>(rootParams.size()),
			rootParams.data(),
			0,
			nullptr,
			D3D12_ROOT_SIGNATURE_FLAG_NONE
		};

		ComPtr<ID3DBlob> globalRootSigBlob, errorBlob;
		HR(D3D12SerializeRootSignature(&globalRootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &globalRootSigBlob, &errorBlob));
		HR(device->CreateRootSignature(0, globalRootSigBlob->GetBufferPointer(), globalRootSigBlob->GetBufferSize(), IID_PPV_ARGS(&m_GlobalRootSignature)));

		// TODO: Test implementation
		D3D12_ROOT_SIGNATURE_DESC localRootSigDesc =
		{
			0,
			nullptr,
			0,
			nullptr,
			D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE
		};

		ComPtr<ID3DBlob> localRootSigBlob;
		HR(D3D12SerializeRootSignature(&localRootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &localRootSigBlob, &errorBlob));
		HR(device->CreateRootSignature(0, localRootSigBlob->GetBufferPointer(), localRootSigBlob->GetBufferSize(), IID_PPV_ARGS(&m_LocalRootSignature)));

		std::vector<String> entrypointNames;
		entrypointNames.reserve(shaders.size());
		std::vector<D3D12_SHADER_BYTECODE> shaderBytecodes;
		shaderBytecodes.reserve(shaders.size());
		std::vector<D3D12_DXIL_LIBRARY_DESC> dxilLibDesc;
		dxilLibDesc.reserve(shaders.size());
		std::vector<D3D12_EXPORT_DESC> exports;
		exports.reserve(shaders.size());
		std::vector<D3D12_STATE_SUBOBJECT> subobjects;
		subobjects.reserve(shaders.size() + 5);
		for (size_t i = 0; i < shaders.size(); ++i)
		{
			auto& shader = shaders[i];
			auto& entrypointName = entrypointNames.emplace_back(shader->GetEntrypointName());
			auto& shaderBytecode = shaderBytecodes.emplace_back(D3D12_SHADER_BYTECODE
			{
				.pShaderBytecode = shader->GetBytecode(),
				.BytecodeLength = shader->GetBytecodeSize()
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
		hitGroupDesc.HitGroupExport = L"DefaultHit";
		hitGroupDesc.Type = D3D12_HIT_GROUP_TYPE_TRIANGLES;
		hitGroupDesc.ClosestHitShaderImport = L"DefaultClosestHit";

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
			.NumSubobjects = (UINT)subobjects.size(),
			.pSubobjects = subobjects.data()
		};
		HR(device->CreateStateObject(&stateObjectDesc, IID_PPV_ARGS(&m_StateObject)));
	}
}