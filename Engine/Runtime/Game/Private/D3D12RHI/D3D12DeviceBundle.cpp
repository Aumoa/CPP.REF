// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12DeviceBundle.h"

#include "Logging/Logger.h"
#include "Logging/LogMacros.h"
#include "Application.h"
#include "D3D12SwapChain.h"
#include "D3D12ImmediateCommandList.h"
#include "D3D12CommandFence.h"
#include "D3D12RenderTargetView.h"
#include "D3D12OfflineDescriptorManager.h"
#include "D3D12OfflineDescriptorIndex.h"
#include "D3D12OnlineDescriptorManager.h"
#include "D3D12OnlineDescriptorIndex.h"
#include "D3D12Resource.h"
#include "D3D12DeferredCommandList.h"
#include "D3D12Shader.h"
#include "D3D12Fence.h"
#include "D3D12DepthStencilView.h"
#include "D3D12ShaderResourceView.h"
#include "RHI/RHIShaderLibrary.h"
#include "RHI/RHIVertex.h"
#include "RHI/RHIResourceGC.h"
#include "RHI/RHITextureClearValue.h"

using namespace std;

D3D12DeviceBundle* D3D12DeviceBundle::instance = nullptr;

D3D12DeviceBundle::D3D12DeviceBundle() : Super()
{
#ifdef _DEBUG
	if (ComPtr<ID3D12Debug> debug; SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
	{
		debug->EnableDebugLayer();
		SE_LOG(LogD3D12RHI, Verbose, "D3D12 debug layer enabled.");
	}
	else
	{
		SE_LOG(LogD3D12RHI, Warning, "Cannot enable D3D12 debug layer.");
	}
#endif
}

D3D12DeviceBundle::~D3D12DeviceBundle()
{
	CoUninitialize();
}

void D3D12DeviceBundle::InitializeBundle()
{
	if (instance != nullptr)
	{
		SE_LOG(LogD3D12RHI, Error, "Bundle duplicate detected.");
		throw HResultException(E_FAIL);
	}

	SE_LOG(LogD3D12RHI, Verbose, "Initialize D3D12 device bundle.");

	InitializeCOM();
	InitializeDXGI();
	InitializeD3D12();
	InitializeShaders();

	resourceGC = NewObject<RHIResourceGC>();

	instance = this;

	GApplication.Sizing += bind_delegate(Application_OnSizing);
}

void D3D12DeviceBundle::ReleaseBundle()
{
	instance = nullptr;
}

IRHISwapChain* D3D12DeviceBundle::GetSwapChain() const
{
	return swapChain.Get();
}

IRHIImmediateCommandList* D3D12DeviceBundle::GetImmediateCommandList() const
{
	return immediateCommandList.Get();
}

RHIShaderLibrary* D3D12DeviceBundle::GetShaderLibrary() const
{
	return shaderLibrary.Get();
}

RHIResourceGC* D3D12DeviceBundle::GetResourceGC() const
{
	return resourceGC.Get();
}

TRefPtr<IRHICommandFence> D3D12DeviceBundle::CreateCommandFence()
{
	return NewObject<D3D12CommandFence>(d3d12Device.Get());
}

TRefPtr<IRHIRenderTargetView> D3D12DeviceBundle::CreateRenderTargetView(IRHIResource* resource)
{
	ID3D12Resource* resource1 = Cast<D3D12Resource>(resource)->Resource;
	D3D12OfflineDescriptorIndex index = rtvManager->Alloc();
	d3d12Device->CreateRenderTargetView(resource1, nullptr, index.Handle);
	return NewObject<D3D12RenderTargetView>(resource1, index);
}

TRefPtr<IRHIDepthStencilView> D3D12DeviceBundle::CreateDepthStencilView(IRHIResource* resource, ERHITextureFormat inViewFormat)
{
	if (inViewFormat == ERHITextureFormat::Unknown)
	{
		inViewFormat = resource->GetDesc().Format;
	}

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{ };
	dsvDesc.Format = (DXGI_FORMAT)inViewFormat;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	ID3D12Resource* resource1 = Cast<D3D12Resource>(resource)->Resource;
	D3D12OfflineDescriptorIndex index = dsvManager->Alloc();
	d3d12Device->CreateDepthStencilView(resource1, &dsvDesc, index.Handle);
	return NewObject<D3D12DepthStencilView>(resource1, index);
}

TRefPtr<IRHIShaderResourceView> D3D12DeviceBundle::CreateTextureView(IRHIResource* resource, ERHITextureFormat inViewFormat)
{
	if (inViewFormat == ERHITextureFormat::Unknown)
	{
		inViewFormat = resource->GetDesc().Format;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{ };
	srvDesc.Format = (DXGI_FORMAT)inViewFormat;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = 1;

	ID3D12Resource* resource1 = Cast<D3D12Resource>(resource)->Resource;
	D3D12OnlineDescriptorIndex index = srvManager->Alloc();
	d3d12Device->CreateShaderResourceView(resource1, &srvDesc, index.Handle);
	return NewObject<D3D12ShaderResourceView>(resource1, index);
}

TRefPtr<IRHIResource> D3D12DeviceBundle::CreateTexture2D(ERHITextureFormat format, int32 width, int32 height, ERHIResourceStates initialStates, ERHIResourceFlags flags, const RHITextureClearValue& inClearValue)
{
	D3D12_HEAP_PROPERTIES heapProp{ };
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_RESOURCE_DESC desc{ };
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Width = (UINT64)width;
	desc.Height = (UINT)height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = (DXGI_FORMAT)format;
	desc.SampleDesc = { 1, 0 };
	desc.Flags = (D3D12_RESOURCE_FLAGS)flags;

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = (DXGI_FORMAT)inClearValue.Format;
	if (inClearValue.IsColor)
	{
		memcpy(clearValue.Color, &inClearValue.Color.R, sizeof(Color));
	}
	else if (inClearValue.IsDepthStencil)
	{
		clearValue.DepthStencil.Depth = inClearValue.Depth;
		clearValue.DepthStencil.Stencil = inClearValue.Stencil;
	}

	ComPtr<ID3D12Resource> resource;
	HR(d3d12Device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &desc, (D3D12_RESOURCE_STATES)initialStates, inClearValue.IsValid ? &clearValue : nullptr, IID_PPV_ARGS(&resource)));

	return NewObject<D3D12Resource>(resource.Get());
}

TRefPtr<IRHIDeferredCommandList> D3D12DeviceBundle::CreateDeferredCommandList()
{
	return NewObject<D3D12DeferredCommandList>(d3d12Device.Get());
}

TRefPtr<IRHIFence> D3D12DeviceBundle::CreateFence()
{
	return NewObject<D3D12Fence>();
}

TRefPtr<IRHIResource> D3D12DeviceBundle::CreateVertexBuffer(span<RHIVertex> vertices)
{
	span<uint8> buffer = span((uint8*)vertices.data(), vertices.size() * sizeof(RHIVertex));
	ComPtr<ID3D12Resource> resource = CreateImmutableBuffer(D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, buffer);
	return NewObject<D3D12Resource>(resource.Get());
}

TRefPtr<IRHIResource> D3D12DeviceBundle::CreateIndexBuffer(span<uint32> indices)
{
	span<uint8> buffer = span((uint8*)indices.data(), indices.size() * sizeof(uint32));
	ComPtr<ID3D12Resource> resource = CreateImmutableBuffer(D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, buffer);
	return NewObject<D3D12Resource>(resource.Get());
}

TRefPtr<IRHIResource> D3D12DeviceBundle::CreateDynamicConstantBuffer(size_t sizeInBytes)
{
	D3D12_RESOURCE_DESC bufferDesc = { };
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = (UINT64)sizeInBytes;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
	bufferDesc.SampleDesc = { 1, 0 };
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	bufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	D3D12_HEAP_PROPERTIES heap = { };
	heap.Type = D3D12_HEAP_TYPE_UPLOAD;

	ComPtr<ID3D12Resource> resource;
	HR(d3d12Device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource)));

	auto d3d12Res = NewObject<D3D12Resource>(resource.Get());
	d3d12Res->BindMappingAddress();

	return move(d3d12Res);
}

ID3D12Device* D3D12DeviceBundle::Device_get() const
{
	return d3d12Device.Get();
}

D3D12OnlineDescriptorManager* D3D12DeviceBundle::SrvManager_get() const
{
	return srvManager.Get();
}

void D3D12DeviceBundle::InitializeCOM()
{
	HR(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
}

void D3D12DeviceBundle::InitializeDXGI()
{
	UINT flag = 0;
#ifdef _DEBUG
	flag |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	HR(CreateDXGIFactory2(flag, IID_PPV_ARGS(&dxgiFactory)));
}

void D3D12DeviceBundle::InitializeD3D12()
{
	constexpr size_t OfflineDescriptorAllocUnit = 128;

	ComPtr<IDXGIAdapter1> adapter;
	for (int32 i = 0; SUCCEEDED(dxgiFactory->EnumAdapters1((UINT)i, &adapter)); ++i)
	{
		if (!IsAdapterSuitable(adapter.Get()))
		{
			continue;
		}

		if (FAILED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&d3d12Device))))
		{
			continue;
		}

		if (!IsDeviceSuitable(d3d12Device.Get()))
		{
			continue;
		}

		DXGI_ADAPTER_DESC1 desc = { };
		HR(adapter->GetDesc1(&desc));
		
		SE_LOG(LogD3D12RHI, Verbose, L"That supported feature level 12_1 device named to {0} is selected.", desc.Description);
		break;
	}

	if (!d3d12Device)
	{
		SE_LOG(LogD3D12RHI, Error, "Failed to detect adapter that support feature level 12_1. Create device with software platform.");
		throw HResultException(CO_E_NOT_SUPPORTED);
	}

	// Create core command queue.
	immediateCommandList = NewObject<D3D12ImmediateCommandList>(d3d12Device.Get());

	DXGI_SWAP_CHAIN_DESC1 chainDesc{ };
	chainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	chainDesc.SampleDesc = { 1, 0 };
	chainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	chainDesc.BufferCount = 3;
	chainDesc.Scaling = DXGI_SCALING_STRETCH;
	chainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	chainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

	HWND hWnd = Application::GetInstance()->GetCoreHwnd();
	ComPtr<IDXGISwapChain1> swapChain;
	HR(dxgiFactory->CreateSwapChainForHwnd(immediateCommandList->CommandQueue, hWnd, &chainDesc, nullptr, nullptr, &swapChain));
	ComPtr<IDXGISwapChain4> swapChain4;
	HR(swapChain.As(&swapChain4));

	this->swapChain = NewObject<D3D12SwapChain>(swapChain4.Get());

	rtvManager = NewObject<D3D12OfflineDescriptorManager>(d3d12Device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, OfflineDescriptorAllocUnit);
	dsvManager = NewObject<D3D12OfflineDescriptorManager>(d3d12Device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, OfflineDescriptorAllocUnit);
	srvManager = NewObject<D3D12OnlineDescriptorManager>(d3d12Device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 4096);
}

#include "Shaders/GeometryShader/Compiled/VertexShader.hlsl.h"
#include "Shaders/GeometryShader/Compiled/PixelShader.hlsl.h"
#include "Shaders/LightingShader/Compiled/VertexShader.hlsl.h"
#include "Shaders/LightingShader/Compiled/PixelShader.hlsl.h"
#include "Shaders/TonemapShader/Compiled/VertexShader.hlsl.h"
#include "Shaders/TonemapShader/Compiled/PixelShader.hlsl.h"

template<class... TArgs, size_t... Indices>
inline void SetRTVFormatsHelper(D3D12_GRAPHICS_PIPELINE_STATE_DESC& outDesc, TArgs&&... inArgs, index_sequence<Indices...>&&)
{
	outDesc.NumRenderTargets = (uint32)sizeof...(TArgs);
	tie(outDesc.RTVFormats[Indices]...) = tie(inArgs...);
}

void D3D12DeviceBundle::InitializeShaders()
{
	static auto GetRootCBVParameter = [](uint32 shaderRegister, D3D12_SHADER_VISIBILITY shaderVisibility)
	{
		D3D12_ROOT_PARAMETER param = { };
		param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		param.ShaderVisibility = shaderVisibility;
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
		D3D12_ROOT_PARAMETER RootParameters[]
		{
			GetRootCBVParameter(0, D3D12_SHADER_VISIBILITY_VERTEX)
		};

		D3D12_INPUT_ELEMENT_DESC InputElements[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		D3D12_ROOT_SIGNATURE_DESC RSDesc = GetRootSignatureDesc(RootParameters, { });
		RSDesc.Flags = GetRSFlagsIA3(HULL, DOMAIN, GEOMETRY);
		ComPtr<ID3D12RootSignature> pRS = CreateRootSignature(RSDesc);

		D3D12_GRAPHICS_PIPELINE_STATE_DESC PSDesc = GetInitialPipelineDesc(pRS.Get());
		PSDesc.VS = GetShaderBytecode(pGeometryVertexShader);
		PSDesc.PS = GetShaderBytecode(pGeometryPixelShader);
		PSDesc.DepthStencilState = { TRUE, D3D12_DEPTH_WRITE_MASK_ALL, D3D12_COMPARISON_FUNC_LESS, FALSE, 0, 0 };
		PSDesc.InputLayout = GetInputLayout(InputElements);
		SetRTVFormats(PSDesc, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_R16G16B16A16_FLOAT);
		PSDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		ComPtr<ID3D12PipelineState> pPS;
		HR(d3d12Device->CreateGraphicsPipelineState(&PSDesc, IID_PPV_ARGS(&pPS)));

		lib->SetShader(RHIShaderLibrary::GeometryShader, NewObject<D3D12Shader>(pRS.Get(), pPS.Get()));
	}

	{  // Lighting Shader
		D3D12_DESCRIPTOR_RANGE ranges1[]
		{
			{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND }
		};

		D3D12_DESCRIPTOR_RANGE ranges2[]
		{
			{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND }
		};

		D3D12_ROOT_PARAMETER RootParameters[]
		{
			GetRootDescriptorTableParameter(D3D12_SHADER_VISIBILITY_PIXEL, ranges1),
			GetRootDescriptorTableParameter(D3D12_SHADER_VISIBILITY_PIXEL, ranges2)
		};

		D3D12_STATIC_SAMPLER_DESC StaticSamplers[]
		{
			GetStaticSampler(D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_BORDER)
		};

		D3D12_ROOT_SIGNATURE_DESC RSDesc = GetRootSignatureDesc(RootParameters, StaticSamplers);
		RSDesc.Flags = GetRSFlags4(VERTEX, DOMAIN, HULL, GEOMETRY);
		ComPtr<ID3D12RootSignature> pRS = CreateRootSignature(RSDesc);

		D3D12_GRAPHICS_PIPELINE_STATE_DESC PSDesc = GetInitialPipelineDesc(pRS.Get());
		PSDesc.VS = GetShaderBytecode(pLightingVertexShader);
		PSDesc.PS = GetShaderBytecode(pLightingPixelShader);
		SetRTVFormats(PSDesc, DXGI_FORMAT_R16G16B16A16_FLOAT);

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
}

bool D3D12DeviceBundle::IsAdapterSuitable(IDXGIAdapter1* adapter) const
{
	DXGI_ADAPTER_DESC1 desc{ };
	HR(adapter->GetDesc1(&desc));

	if (desc.Flags != DXGI_ADAPTER_FLAG_NONE)
	{
		// Is remote or software implement.
		return false;
	}

	return true;
}

bool D3D12DeviceBundle::IsDeviceSuitable(ID3D12Device* device) const
{
	UNREFERENCED_PARAMETER(device);

	return true;
}

ComPtr<ID3D12Resource> D3D12DeviceBundle::CreateImmutableBuffer(D3D12_RESOURCE_STATES initialState, span<uint8> initialBuffer, ERHIResourceFlags flags)
{
	D3D12_RESOURCE_DESC bufferDesc = { };
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = (UINT64)initialBuffer.size_bytes();
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
	bufferDesc.SampleDesc = { 1, 0 };
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	bufferDesc.Flags = (D3D12_RESOURCE_FLAGS)flags;

	D3D12_HEAP_PROPERTIES heap = { };
	heap.Type = D3D12_HEAP_TYPE_DEFAULT;

	ComPtr<ID3D12Resource> resource;
	HR(d3d12Device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&resource)));

	ComPtr<ID3D12Resource> uploadHeap;
	heap.Type = D3D12_HEAP_TYPE_UPLOAD;
	HR(d3d12Device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadHeap)));

	void* pData;
	HR(uploadHeap->Map(0, nullptr, &pData));
	memcpy(pData, initialBuffer.data(), initialBuffer.size_bytes());
	uploadHeap->Unmap(0, nullptr);

	D3D12_RESOURCE_BARRIER barrier = { };
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = resource.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = initialState;

	TRefPtr<D3D12DeferredCommandList> deferredCmd = Cast<D3D12DeferredCommandList>(CreateDeferredCommandList());
	deferredCmd->BeginCommand();
	deferredCmd->CommandList->CopyResource(resource.Get(), uploadHeap.Get());
	deferredCmd->CommandList->ResourceBarrier(1, &barrier);
	deferredCmd->EndCommand();

	uint64 issued = resourceGC->IssueNumber();
	resourceGC->AddPendingResource(deferredCmd, issued);
	resourceGC->AddPendingResource(NewObject<D3D12Resource>(uploadHeap.Get()), issued);

	immediateCommandList->ExecuteCommandList(deferredCmd.Get());
	resourceGC->SignalNumber(issued);

	return move(resource);
}

void D3D12DeviceBundle::Application_OnSizing(int32 width, int32 height)
{
	SE_LOG(LogD3D12RHI, Verbose, L"Swap chain resized to {0} x {1}", width, height);
	swapChain->ResizeBuffers(width, height);
}