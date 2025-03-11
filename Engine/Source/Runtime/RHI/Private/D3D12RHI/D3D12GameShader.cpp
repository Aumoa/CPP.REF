// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12GameShader.h"

#if PLATFORM_WINDOWS

ND3D12GameShader::ND3D12GameShader(ID3D12Device5& InDevice)
{
	D3D12_ROOT_SIGNATURE_DESC RSDesc =
	{
		.NumParameters = 0,
		.NumStaticSamplers = 0
	};

	ComPtr<ID3DBlob> pSerializedBlob, pError;
	if (HRESULT Hr = D3D12SerializeRootSignature(&RSDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &pSerializedBlob, &pError); FAILED(Hr))
	{
		if (pError)
		{
			PlatformProcess::OutputDebugString(String::Format(TEXT("{}\n"), String::FromLiteral((const char*)pError->GetBufferPointer())));
		}
		HR(Hr);
	}

	HR(InDevice.CreateRootSignature(0, pSerializedBlob->GetBufferPointer(), pSerializedBlob->GetBufferSize(), IID_PPV_ARGS(&pRS)));

	D3D12_RAYTRACING_SHADER_CONFIG ShaderConfig =
	{
		.MaxPayloadSizeInBytes = sizeof(Vector3),
		.MaxAttributeSizeInBytes = sizeof(Vector3)
	};

	D3D12_RAYTRACING_PIPELINE_CONFIG PipelineConfig =
	{
		.MaxTraceRecursionDepth = 1
	};

	D3D12_GLOBAL_ROOT_SIGNATURE GlobalRootSignature =
	{
		.pGlobalRootSignature = pRS.Get()
	};

	D3D12_STATE_SUBOBJECT Subobjects[] =
	{
		{ .Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG, .pDesc = &ShaderConfig },
		{ .Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG, .pDesc = &PipelineConfig },
		{ .Type = D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE, .pDesc = &GlobalRootSignature }
	};

	D3D12_STATE_OBJECT_DESC PSDesc =
	{
		.Type = D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE,
		.NumSubobjects = (UINT)AE_ARRAYSIZE(Subobjects),
		.pSubobjects = Subobjects
	};

	HR(InDevice.CreateStateObject(&PSDesc, IID_PPV_ARGS(&pStateObject)));
	HR(pStateObject.As(&pProperties));
}

#endif