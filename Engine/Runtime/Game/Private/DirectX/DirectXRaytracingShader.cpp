// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXRaytracingShader.h"

#include "DirectXCommon.h"
#include "SceneRendering/Vertex.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "Shaders/ShaderTypes.h"
#include "CompiledShaders/RaytracingShader.generated.h"

using namespace std;

namespace
{
	inline auto GetRootCBVParameter(uint32 shaderRegister, D3D12_SHADER_VISIBILITY shaderVisibility)
	{
		D3D12_ROOT_PARAMETER param = { };
		param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		param.ShaderVisibility = shaderVisibility;
		param.Descriptor.ShaderRegister = shaderRegister;
		return param;
	};

	inline auto GetRoot32BitCBVParameter(uint32 shaderRegister, uint32 num32Bits, D3D12_SHADER_VISIBILITY shaderVisibility)
	{
		D3D12_ROOT_PARAMETER param = { };
		param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		param.ShaderVisibility = shaderVisibility;
		param.Constants.Num32BitValues = num32Bits;
		param.Constants.ShaderRegister = shaderRegister;
		return param;
	};

	template<size_t NumRanges>
	inline auto GetRootDescriptorTableParameter(D3D12_SHADER_VISIBILITY shaderVisibility, const D3D12_DESCRIPTOR_RANGE(&inRanges)[NumRanges])
	{
		D3D12_ROOT_PARAMETER param = { };
		param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		param.ShaderVisibility = shaderVisibility;
		param.DescriptorTable.NumDescriptorRanges = (uint32)NumRanges;
		param.DescriptorTable.pDescriptorRanges = inRanges;
		return param;
	};

	inline auto GetRootShaderResourceView(uint32 shaderRegister, D3D12_SHADER_VISIBILITY shaderVisibility, uint32 space = 0)
	{
		D3D12_ROOT_PARAMETER param = { };
		param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
		param.ShaderVisibility = shaderVisibility;
		param.Descriptor.ShaderRegister = shaderRegister;
		param.Descriptor.RegisterSpace = space;
		return param;
	};

	inline auto GetRootSignatureDesc(span<D3D12_ROOT_PARAMETER> inParameters, span<D3D12_STATIC_SAMPLER_DESC> inSamplers)
	{
		D3D12_ROOT_SIGNATURE_DESC RSDesc = { };
		RSDesc.NumParameters = (uint32)inParameters.size();
		RSDesc.pParameters = inParameters.data();
		RSDesc.NumStaticSamplers = (uint32)inSamplers.size();
		RSDesc.pStaticSamplers = inSamplers.data();
		return RSDesc;
	};

	template<size_t NumBytes>
	inline auto GetShaderBytecode(const BYTE(&inArray)[NumBytes]) -> D3D12_SHADER_BYTECODE
	{
		return { inArray, NumBytes };
	};

	template<size_t NumItems>
	inline auto GetInputLayout(const D3D12_INPUT_ELEMENT_DESC(&inArray)[NumItems]) -> D3D12_INPUT_LAYOUT_DESC
	{
		return { inArray, NumItems };
	};

	inline auto GetInitialPipelineDesc(ID3D12RootSignature* pRS)
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PSDesc = { };
		PSDesc.pRootSignature = pRS;
		for (size_t i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
			PSDesc.BlendState.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		}
		PSDesc.SampleMask = 0xFFFFFFFF;
		PSDesc.RasterizerState = { D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK };
		PSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		PSDesc.SampleDesc = { 1, 0 };
		return PSDesc;
	};

	template<class... TArgs, size_t... Indices>
	inline void SetRTVFormatsHelper(D3D12_GRAPHICS_PIPELINE_STATE_DESC& outDesc, TArgs&&... inArgs, index_sequence<Indices...>&&)
	{
		outDesc.NumRenderTargets = (uint32)sizeof...(TArgs);
		tie(outDesc.RTVFormats[Indices]...) = tie(inArgs...);
	}

	template<class... TArgs>
	inline auto SetRTVFormats(D3D12_GRAPHICS_PIPELINE_STATE_DESC & outDesc, TArgs&&... inArgs)
	{
		SetRTVFormatsHelper<TArgs...>(outDesc, forward<TArgs>(inArgs)..., make_index_sequence<sizeof...(TArgs)>{ });
	};

	inline auto GetStaticSampler(D3D12_FILTER inFilter, D3D12_TEXTURE_ADDRESS_MODE inAddressMode, uint32 inRegister = 0, D3D12_SHADER_VISIBILITY inVisibility = D3D12_SHADER_VISIBILITY_PIXEL)
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

	inline auto CreateRootSignature(ID3D12Device5* device, const D3D12_ROOT_SIGNATURE_DESC& inRS)
	{
		TComPtr<ID3DBlob> pBlob, pError;
		TComPtr<ID3D12RootSignature> pRootSignature;
		HR(D3D12SerializeRootSignature(&inRS, D3D_ROOT_SIGNATURE_VERSION_1_0, &pBlob, &pError));
		HR(device->CreateRootSignature(0, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), IID_PPV_ARGS(&pRootSignature)));
		return pRootSignature;
	};
}

DirectXRaytracingShader::DirectXRaytracingShader(DirectXDeviceBundle* deviceBundle) : Super()
	, device(deviceBundle->GetDevice())
{

}

DirectXRaytracingShader::~DirectXRaytracingShader()
{

}

void DirectXRaytracingShader::Init()
{
	InitRS();
	InitLocalRS();
	InitHitGroups();
	InitPS();
}

void DirectXRaytracingShader::Render(ID3D12GraphicsCommandList4* inCommandList)
{
	inCommandList->SetComputeRootSignature(rootSignature.Get());
	inCommandList->SetPipelineState1(pipelineState.Get());
}

const void* DirectXRaytracingShader::GetRayGenerationIdentifier() const
{
	return properties->GetShaderIdentifier(L"RayGeneration");
}

const void* DirectXRaytracingShader::GetClosestHitIdentifier(size_t shaderIndex) const
{
	return properties->GetShaderIdentifier(L"OpaqueHit");
}

const void* DirectXRaytracingShader::GetMissIdentifier(size_t shaderIndex) const
{
	return properties->GetShaderIdentifier(L"Miss");
}

void DirectXRaytracingShader::InitRS()
{
	D3D12_DESCRIPTOR_RANGE colorOutput[] =
	{
		{ D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND },
	};

	D3D12_ROOT_PARAMETER rootParameters[] =
	{
		GetRootCBVParameter(0, D3D12_SHADER_VISIBILITY_ALL),
		GetRootDescriptorTableParameter(D3D12_SHADER_VISIBILITY_ALL, colorOutput),
		GetRootShaderResourceView(0, D3D12_SHADER_VISIBILITY_ALL),
	};

	rootSignature = CreateRootSignature(device, GetRootSignatureDesc(rootParameters, { }));
}

void DirectXRaytracingShader::InitLocalRS()
{
	D3D12_ROOT_PARAMETER rootParams[] =
	{
		GetRootShaderResourceView(0, D3D12_SHADER_VISIBILITY_ALL, 2),
		GetRootShaderResourceView(1, D3D12_SHADER_VISIBILITY_ALL, 2),
	};

	D3D12_ROOT_SIGNATURE_DESC localRSDesc = GetRootSignatureDesc(rootParams, { });
	localRSDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
	localRS.emplace_back(CreateRootSignature(device, localRSDesc));
}

void DirectXRaytracingShader::InitHitGroups()
{
	hitGroups.emplace_back(
		L"OpaqueHit",
		D3D12_HIT_GROUP_TYPE_TRIANGLES,
		/* AnyHit */ nullptr,
		L"ClosestHit",
		/* Intersection */ nullptr
	);
}

void DirectXRaytracingShader::InitPS()
{
	// Export shader function from HLSL shader library using shader signature.
	D3D12_EXPORT_DESC exportDesc[] =
	{
		{ L"RayGeneration", nullptr, D3D12_EXPORT_FLAG_NONE },
		{ L"Miss", nullptr, D3D12_EXPORT_FLAG_NONE },
		{ L"ClosestHit", nullptr, D3D12_EXPORT_FLAG_NONE },
	};

	D3D12_DXIL_LIBRARY_DESC exportShaders =
	{
		GetShaderBytecode(pRaytracingShader),
		_countof(exportDesc),
		exportDesc
	};

	// Raytracing shader configuration.
	D3D12_RAYTRACING_SHADER_CONFIG shaderConfig =
	{
		sizeof(Payload),
		sizeof(BuiltInAttr)
	};

	D3D12_RAYTRACING_PIPELINE_CONFIG pipelineConfig =
	{
		/* MaxTraceRecursionDepth */
		1,
	};

	std::vector<D3D12_STATE_SUBOBJECT> state_hitGroups;
	for (size_t i = 0; i < hitGroups.size(); ++i)
	{
		state_hitGroups.emplace_back(
			D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP,
			&hitGroups[i]
		);
	}

	// Populate local root signature description.
	std::vector<D3D12_STATE_SUBOBJECT> state_localRS;
	std::vector<D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION> localRSAsso;
	std::vector<D3D12_STATE_SUBOBJECT> state_localRSAsso;
	for (size_t i = 0; i < localRS.size(); ++i)
	{
		LPCWSTR opaqueHit = L"OpaqueHit";

		state_localRS.emplace_back(D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE, (void*)localRS[0].GetAddressOf());
		localRSAsso.emplace_back(
			&state_localRS.back(),
			1,
			&opaqueHit
		);
		state_localRSAsso.emplace_back(D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION, &localRSAsso.back());
	}

	// Composite raytracing shader subobjects.
	std::vector<D3D12_STATE_SUBOBJECT> state_subobjects =
	{
		/* shader */ { D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY, &exportShaders },
		/* shaderConfig */ { D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG, &shaderConfig },
		/* globalRs */ { D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE, rootSignature.GetAddressOf() },
		/* pipelineConfig */ { D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG, &pipelineConfig },
	};

	auto state_append = [&state_subobjects](auto& container)
	{
		state_subobjects.insert(state_subobjects.end(), container.begin(), container.end());
	};

	state_append(state_hitGroups);
	state_append(state_localRS);
	state_append(state_localRSAsso);

	D3D12_STATE_OBJECT_DESC pipelineSubobjects =
	{
		D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE,
		(uint32)state_subobjects.size(),
		state_subobjects.data()
	};

	HR(device->CreateStateObject(&pipelineSubobjects, IID_PPV_ARGS(&pipelineState)));
	HR(pipelineState->QueryInterface(IID_PPV_ARGS(&properties)));
}