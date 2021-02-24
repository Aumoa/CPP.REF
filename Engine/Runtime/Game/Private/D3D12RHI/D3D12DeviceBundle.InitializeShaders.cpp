// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12DeviceBundle.h"

#include "D3D12Shader.h"
#include "RHI/RHIShaderLibrary.h"
#include "Logging/LogMacros.h"
#include "CompiledShaders/GeometryVertexShader.generated.h"
#include "CompiledShaders/GeometryPixelShader.generated.h"
#include "CompiledShaders/LightingVertexShader.generated.h"
#include "CompiledShaders/LightingPixelShader.generated.h"
#include "CompiledShaders/LightingRayGeneration.generated.h"
#include "CompiledShaders/TonemapVertexShader.generated.h"
#include "CompiledShaders/TonemapPixelShader.generated.h"

using namespace std;

template<class... TArgs, size_t... Indices>
inline void SetRTVFormatsHelper(D3D12_GRAPHICS_PIPELINE_STATE_DESC& outDesc, TArgs&&... inArgs, index_sequence<Indices...>&&)
{
	outDesc.NumRenderTargets = (uint32)sizeof...(TArgs);
	tie(outDesc.RTVFormats[Indices]...) = tie(inArgs...);
}

void D3D12DeviceBundle::InitializeShaders()
{
	SE_LOG(LogD3D12RHI, Display, L"Initialize engine default shaders.");

	static auto GetRootCBVParameter = [](uint32 shaderRegister, D3D12_SHADER_VISIBILITY shaderVisibility)
	{
		D3D12_ROOT_PARAMETER param = { };
		param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		param.ShaderVisibility = shaderVisibility;
		param.Descriptor.ShaderRegister = shaderRegister;
		return param;
	};

	static auto GetRoot32BitCBVParameter = [](uint32 shaderRegister, uint32 num32Bits, D3D12_SHADER_VISIBILITY shaderVisibility)
	{
		D3D12_ROOT_PARAMETER param = { };
		param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		param.ShaderVisibility = shaderVisibility;
		param.Constants.Num32BitValues = num32Bits;
		param.Constants.ShaderRegister = shaderRegister;
		return param;
	};

	static auto GetRootDescriptorTableParameter = [] <size_t NumRanges> (D3D12_SHADER_VISIBILITY shaderVisibility, const D3D12_DESCRIPTOR_RANGE(&inRanges)[NumRanges])
	{
		D3D12_ROOT_PARAMETER param = { };
		param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		param.ShaderVisibility = shaderVisibility;
		param.DescriptorTable.NumDescriptorRanges = (uint32)NumRanges;
		param.DescriptorTable.pDescriptorRanges = inRanges;
		return param;
	};

	static auto GetRootShaderResourceView = [](uint32 shaderRegister, D3D12_SHADER_VISIBILITY shaderVisibility)
	{
		D3D12_ROOT_PARAMETER param = { };
		param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
		param.ShaderVisibility = shaderVisibility;
		param.Descriptor.ShaderRegister = shaderRegister;
		return param;
	};

	static auto GetRootSignatureDesc = [](span<D3D12_ROOT_PARAMETER> inParameters, span<D3D12_STATIC_SAMPLER_DESC> inSamplers)
	{
		D3D12_ROOT_SIGNATURE_DESC RSDesc = { };
		RSDesc.NumParameters = (uint32)inParameters.size();
		RSDesc.pParameters = inParameters.data();
		RSDesc.NumStaticSamplers = (uint32)inSamplers.size();
		RSDesc.pStaticSamplers = inSamplers.data();
		return RSDesc;
	};

	static auto GetShaderBytecode = [] <size_t NumBytes> (const BYTE(&inArray)[NumBytes]) -> D3D12_SHADER_BYTECODE
	{
		return { inArray, NumBytes };
	};

	static auto GetInputLayout = [] <size_t NumItems> (const D3D12_INPUT_ELEMENT_DESC(&inArray)[NumItems]) -> D3D12_INPUT_LAYOUT_DESC
	{
		return { inArray, NumItems };
	};

	static auto GetInitialPipelineDesc = [](ID3D12RootSignature* pRS)
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PSDesc = { };
		PSDesc.pRootSignature = pRS;
		for (size_t i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
		{
			PSDesc.BlendState.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		}
		PSDesc.SampleMask = 0xFFFFFFFF;
		PSDesc.RasterizerState = { D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK };
		PSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		PSDesc.SampleDesc = { 1, 0 };
		return PSDesc;
	};

	static auto SetRTVFormats = [] <class... TArgs> (D3D12_GRAPHICS_PIPELINE_STATE_DESC& outDesc, TArgs&&... inArgs)
	{
		SetRTVFormatsHelper<TArgs...>(outDesc, forward<TArgs>(inArgs)..., make_index_sequence<sizeof...(TArgs)>{ });
	};

	static auto GetStaticSampler = [](D3D12_FILTER inFilter, D3D12_TEXTURE_ADDRESS_MODE inAddressMode, uint32 inRegister = 0, D3D12_SHADER_VISIBILITY inVisibility = D3D12_SHADER_VISIBILITY_PIXEL)
	{
		D3D12_STATIC_SAMPLER_DESC SSDesc = { };
		SSDesc.Filter = inFilter;
		SSDesc.AddressU = inAddressMode;
		SSDesc.AddressV = inAddressMode;
		SSDesc.AddressW = inAddressMode;
		SSDesc.ShaderRegister = inRegister;
		SSDesc.ShaderVisibility = inVisibility;
		return SSDesc;
	};

	auto CreateRootSignature = [&](const D3D12_ROOT_SIGNATURE_DESC& inRS)
	{
		ComPtr<ID3DBlob> pBlob, pError;
		ComPtr<ID3D12RootSignature> pRootSignature;
		HR(D3D12SerializeRootSignature(&inRS, D3D_ROOT_SIGNATURE_VERSION_1_0, &pBlob, &pError));
		HR(d3d12Device->CreateRootSignature(0, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), IID_PPV_ARGS(&pRootSignature)));
		return pRootSignature;
	};

	shaderLibrary = NewObject<RHIShaderLibrary>();
	RHIShaderLibrary* lib = shaderLibrary.Get();

#define GetRSFlags1(Deny1) D3D12_ROOT_SIGNATURE_FLAG_DENY_ ## Deny1 ## _SHADER_ROOT_ACCESS
#define GetRSFlags2(Deny1, Deny2) GetRSFlags1(Deny1) | GetRSFlags1(Deny2)
#define GetRSFlags3(Deny1, Deny2, Deny3) GetRSFlags2(Deny1, Deny2) | GetRSFlags1(Deny3)
#define GetRSFlags4(Deny1, Deny2, Deny3, Deny4) GetRSFlags3(Deny1, Deny2, Deny3) | GetRSFlags1(Deny4)
#define GetRSFlags5(Deny1, Deny2, Deny3, Deny4, Deny5) GetRSFlags4(Deny1, Deny2, Deny3, Deny4) | GetRSFlags1(Deny5)
#define GetRSFlagsIA1(Deny1) D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | GetRSFlags1(Deny1)
#define GetRSFlagsIA2(Deny1, Deny2) GetRSFlagsIA1(Deny1) | GetRSFlags1(Deny2)
#define GetRSFlagsIA3(Deny1, Deny2, Deny3) GetRSFlagsIA2(Deny1, Deny2) | GetRSFlags1(Deny3)
#define GetRSFlagsIA4(Deny1, Deny2, Deny3, Deny4) GetRSFlagsIA3(Deny1, Deny2, Deny3) | GetRSFlags1(Deny4)
#define GetRSFlagsIA5(Deny1, Deny2, Deny3, Deny4, Deny5) GetRSFlagsIA4(Deny1, Deny2, Deny3, Deny4) | GetRSFlags1(Deny5)
#undef DOMAIN

	{  // Geometry Shader
		D3D12_DESCRIPTOR_RANGE ranges[] =
		{
			{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND }
		};

		D3D12_ROOT_PARAMETER RootParameters[]
		{
			GetRootCBVParameter(0, D3D12_SHADER_VISIBILITY_VERTEX),
			GetRoot32BitCBVParameter(0, 1, D3D12_SHADER_VISIBILITY_PIXEL),
			GetRootDescriptorTableParameter(D3D12_SHADER_VISIBILITY_PIXEL, ranges)
		};

		D3D12_STATIC_SAMPLER_DESC staticSamplers[] =
		{
			GetStaticSampler(D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_WRAP)
		};

		D3D12_INPUT_ELEMENT_DESC InputElements[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		D3D12_ROOT_SIGNATURE_DESC RSDesc = GetRootSignatureDesc(RootParameters, staticSamplers);
		RSDesc.Flags = GetRSFlagsIA3(HULL, DOMAIN, GEOMETRY);
		ComPtr<ID3D12RootSignature> pRS = CreateRootSignature(RSDesc);

		D3D12_GRAPHICS_PIPELINE_STATE_DESC PSDesc = GetInitialPipelineDesc(pRS.Get());
		PSDesc.VS = GetShaderBytecode(pGeometryVertexShader);
		PSDesc.PS = GetShaderBytecode(pGeometryPixelShader);
		PSDesc.DepthStencilState = { TRUE, D3D12_DEPTH_WRITE_MASK_ALL, D3D12_COMPARISON_FUNC_LESS, FALSE, 0, 0 };
		PSDesc.InputLayout = GetInputLayout(InputElements);
		SetRTVFormats(PSDesc, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_R16G16B16A16_UINT);
		PSDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		ComPtr<ID3D12PipelineState> pPS;
		HR(d3d12Device->CreateGraphicsPipelineState(&PSDesc, IID_PPV_ARGS(&pPS)));

		lib->SetShader(RHIShaderLibrary::GeometryShader, NewObject<D3D12Shader>(pRS.Get(), pPS.Get()));
	}

	{  // Lighting Shader
		D3D12_DESCRIPTOR_RANGE ranges1[]{ { D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 3, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND } };

		D3D12_ROOT_PARAMETER RootParameters[]
		{
			GetRootCBVParameter(0, D3D12_SHADER_VISIBILITY_PIXEL),
			GetRootDescriptorTableParameter(D3D12_SHADER_VISIBILITY_PIXEL, ranges1),
			GetRootCBVParameter(1, D3D12_SHADER_VISIBILITY_PIXEL),
			GetRootShaderResourceView(3, D3D12_SHADER_VISIBILITY_PIXEL),
		};

		D3D12_STATIC_SAMPLER_DESC StaticSamplers[]
		{
			GetStaticSampler(D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_BORDER)
		};

		D3D12_RENDER_TARGET_BLEND_DESC RTBlendAcc =
		{
			TRUE,
			FALSE,
			D3D12_BLEND_ONE,
			D3D12_BLEND_ONE,
			D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE,
			D3D12_BLEND_ONE,
			D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_CLEAR,
			D3D12_COLOR_WRITE_ENABLE_ALL
		};

		D3D12_RENDER_TARGET_BLEND_DESC RTBlendMul =
		{
			TRUE,
			FALSE,
			D3D12_BLEND_DEST_COLOR,
			D3D12_BLEND_ZERO,
			D3D12_BLEND_OP_ADD,
			D3D12_BLEND_SRC_ALPHA,
			D3D12_BLEND_ZERO,
			D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_CLEAR,
			D3D12_COLOR_WRITE_ENABLE_ALL
		};

		D3D12_ROOT_SIGNATURE_DESC RSDesc = GetRootSignatureDesc(RootParameters, StaticSamplers);
		RSDesc.Flags = GetRSFlags4(VERTEX, DOMAIN, HULL, GEOMETRY);
		ComPtr<ID3D12RootSignature> pRS = CreateRootSignature(RSDesc);

		D3D12_GRAPHICS_PIPELINE_STATE_DESC PSDesc = GetInitialPipelineDesc(pRS.Get());
		PSDesc.VS = GetShaderBytecode(pLightingVertexShader);
		PSDesc.PS = GetShaderBytecode(pLightingPixelShader);
		SetRTVFormats(PSDesc, DXGI_FORMAT_R16G16B16A16_FLOAT);

		PSDesc.BlendState.RenderTarget[0] = RTBlendAcc;
		ComPtr<ID3D12PipelineState> pPS;
		HR(d3d12Device->CreateGraphicsPipelineState(&PSDesc, IID_PPV_ARGS(&pPS)));
		lib->SetShader(RHIShaderLibrary::LightingShader, NewObject<D3D12Shader>(pRS.Get(), pPS.Get()));
	}

	{  // Tonemap Shader
		D3D12_DESCRIPTOR_RANGE ranges1[]
		{
			{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND }
		};

		D3D12_ROOT_PARAMETER RootParameters[]
		{
			GetRootDescriptorTableParameter(D3D12_SHADER_VISIBILITY_PIXEL, ranges1)
		};

		D3D12_STATIC_SAMPLER_DESC StaticSamplers[]
		{
			GetStaticSampler(D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_BORDER)
		};

		D3D12_ROOT_SIGNATURE_DESC RSDesc = GetRootSignatureDesc(RootParameters, StaticSamplers);
		RSDesc.Flags = GetRSFlags4(VERTEX, DOMAIN, HULL, GEOMETRY);
		ComPtr<ID3D12RootSignature> pRS = CreateRootSignature(RSDesc);

		D3D12_GRAPHICS_PIPELINE_STATE_DESC PSDesc = GetInitialPipelineDesc(pRS.Get());
		PSDesc.VS = GetShaderBytecode(pTonemapVertexShader);
		PSDesc.PS = GetShaderBytecode(pTonemapPixelShader);
		SetRTVFormats(PSDesc, DXGI_FORMAT_B8G8R8A8_UNORM);

		ComPtr<ID3D12PipelineState> pPS;
		HR(d3d12Device->CreateGraphicsPipelineState(&PSDesc, IID_PPV_ARGS(&pPS)));

		lib->SetShader(RHIShaderLibrary::TonemapShader, NewObject<D3D12Shader>(pRS.Get(), pPS.Get()));
	}

	{  // Lighting Shader
		D3D12_DESCRIPTOR_RANGE rtv[] =
		{
			{ D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND }
		};

		D3D12_DESCRIPTOR_RANGE srv[] =
		{
			{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 3, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND }
		};

		D3D12_ROOT_PARAMETER RootParameters[] =
		{
			GetRootDescriptorTableParameter(D3D12_SHADER_VISIBILITY_ALL, rtv),
			GetRootCBVParameter(0, D3D12_SHADER_VISIBILITY_ALL),
			GetRootDescriptorTableParameter(D3D12_SHADER_VISIBILITY_ALL, srv),
			GetRootCBVParameter(1, D3D12_SHADER_VISIBILITY_ALL),
			GetRootShaderResourceView(3, D3D12_SHADER_VISIBILITY_ALL),
		};

		D3D12_ROOT_SIGNATURE_DESC RSDesc = GetRootSignatureDesc(RootParameters, { });
		ComPtr<ID3D12RootSignature> pRS = CreateRootSignature(RSDesc);

		D3D12_EXPORT_DESC exportDesc[] =
		{
			{ L"LightingRayGeneration", nullptr, D3D12_EXPORT_FLAG_NONE }
		};

		D3D12_DXIL_LIBRARY_DESC lightingRayGeneration =
		{
			GetShaderBytecode(pLightingRayGeneration),
			1,
			exportDesc
		};

		D3D12_STATE_SUBOBJECT shaders =
		{
			D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY,
			&lightingRayGeneration
		};

		D3D12_RAYTRACING_SHADER_CONFIG shaderConfigDesc =
		{
			0,
			sizeof(Vector2)
		};

		D3D12_STATE_SUBOBJECT shaderConfig =
		{
			D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG,
			&shaderConfigDesc
		};

		D3D12_STATE_SUBOBJECT globalRS =
		{
			D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE,
			pRS.GetAddressOf()
		};

		D3D12_RAYTRACING_PIPELINE_CONFIG pipelineDesc =
		{
			1
		};

		D3D12_STATE_SUBOBJECT pipeline =
		{
			D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG,
			&pipelineDesc,
		};

		D3D12_STATE_SUBOBJECT subobjects[] =
		{
			shaders,
			shaderConfig,
			globalRS,
			pipeline
		};

		D3D12_STATE_OBJECT_DESC pipelineObject =
		{
			D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE,
			4,
			subobjects
		};

		ComPtr<ID3D12StateObject> pStateObject;
		ComPtr<ID3D12StateObjectProperties> pStateObjectProperties;
		HR(d3d12Device->CreateStateObject(&pipelineObject, IID_PPV_ARGS(&pStateObject)));
		HR(pStateObject->QueryInterface(IID_PPV_ARGS(&pStateObjectProperties)));

		lib->SetShader(RHIShaderLibrary::LightingExperimental, NewObject<D3D12Shader>(pRS.Get(), pStateObject.Get(), pStateObjectProperties.Get()));
	}
}