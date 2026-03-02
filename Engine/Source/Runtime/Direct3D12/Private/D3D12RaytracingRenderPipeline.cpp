// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12RaytracingRenderPipeline.h"
#include "D3D12Shader.h"
#include "RaytracePayload.h"
#include "RaytraceAttributes.h"

namespace Ayla
{
	D3D12RaytracingRenderPipeline::D3D12RaytracingRenderPipeline(ID3D12Device5* device, std::vector<SharedPtr<Shader>> shaders)
	{
		D3D12_ROOT_SIGNATURE_DESC globalRootSigDesc =
		{
			0,
			nullptr,
			0,
			nullptr,
			D3D12_ROOT_SIGNATURE_FLAG_NONE
		};

		ComPtr<ID3DBlob> globalRootSigBlob, errorBlob;
		HR(D3D12SerializeRootSignature(&globalRootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &globalRootSigBlob, &errorBlob));

		ComPtr<ID3D12RootSignature> globalRootSignature;
		HR(device->CreateRootSignature(0, globalRootSigBlob->GetBufferPointer(), globalRootSigBlob->GetBufferSize(), IID_PPV_ARGS(&globalRootSignature)));

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

		ComPtr<ID3D12RootSignature> localRootSignature;
		HR(device->CreateRootSignature(0, localRootSigBlob->GetBufferPointer(), localRootSigBlob->GetBufferSize(), IID_PPV_ARGS(&localRootSignature)));

		D3D12_STATE_OBJECT_DESC stateObjectDesc =
		{
			.Type = D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE,
			.NumSubobjects = 0
		};

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
		globalRootSigSubobject.pDesc = globalRootSignature.GetAddressOf();
		subobjects.emplace_back(globalRootSigSubobject);

		D3D12_STATE_SUBOBJECT localRootSigSubobject = {};
		localRootSigSubobject.Type = D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
		localRootSigSubobject.pDesc = localRootSignature.GetAddressOf();
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

		stateObjectDesc.NumSubobjects = (UINT)subobjects.size();
		stateObjectDesc.pSubobjects = subobjects.data();

		HR(device->CreateStateObject(&stateObjectDesc, IID_PPV_ARGS(&m_StateObject)));
	}
}