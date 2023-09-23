// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12RootSignature.h"

#if PLATFORM_WINDOWS

#include "Numerics/MatrixInterface/Matrix2x2.h"
#include "Numerics/VectorInterface/Color.h"

ND3D12RootSignature::ND3D12RootSignature(ID3D12Device& InDevice)
{
	D3D12_DESCRIPTOR_RANGE RS1Ranges[] =
	{
		{
			.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
			.NumDescriptors = 1,
			.BaseShaderRegister = 0,
			.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND,
		}
	};

	D3D12_ROOT_PARAMETER RSParameters[] =
	{
		{
			// ScreenInformation for VS
			.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS,
			.Constants =
			{
				.Num32BitValues = 2
			},
			.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX
		},
		{
			// PaintGeometry for VS
			.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV,
			.Descriptor =
			{
				.ShaderRegister = 1
			},
			.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX
		}
	};

	D3D12_STATIC_SAMPLER_DESC RSStaticParameters[] =
	{
		{
			.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER,
			.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL
		}
	};

	D3D12_ROOT_SIGNATURE_DESC RSDesc =
	{
		.NumParameters = (UINT)AE_ARRAYSIZE(RSParameters),
		.pParameters = RSParameters,
		.NumStaticSamplers = 1,
		.pStaticSamplers = RSStaticParameters,
		.Flags = D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS
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
}

#endif