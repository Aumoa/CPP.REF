// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12Device.h"
#include "D3D12CommandQueue.h"
#include "D3D12ThreadAllocatorContainer.h"
#include "D3D12CommandList.h"
#include "D3D12Texture2D.h"
#include "D3D12Buffer.h"
#include "D3D12Shader.h"
#include "D3D12ThreadDescriptorHeapContainer.h"
#include "D3D12RenderTargetView.h"
#include "D3D12DepthStencilView.h"
#include "D3D12ShaderResourceView.h"
#include "DXGIFactory.h"
#include "D2D1DeviceContext.h"
#include "D2D1SolidColorBrush.h"
#include "D2D1Bitmap.h"
#include "IApplicationInterface.h"
#include "Materials/Material.h"
#include "Threading/Thread.h"

GENERATE_BODY(SD3D12Device);

SD3D12Device::SD3D12Device(SDXGIFactory* InFactory, ComPtr<ID3D12Device> device) : Super(InFactory)
	, _device(std::move(device))
{
	AllocateCommandQueue();
	CreateInteropDevice();
}

SD3D12Device::~SD3D12Device()
{
	if (_immCon)
	{
		// Flush and signal forcely.
		_immCon->ExecuteCommandLists({}, true);
		_immCon->WaitCompleted();
	}
}

IRHIDeviceContext* SD3D12Device::GetImmediateContext()
{
	return _immCon;
}

IRHIDeviceContext* SD3D12Device::CreateDeviceContext()
{
	return NewObject<SD3D12CommandList>(Factory, this);
}

IRHIDeviceContext2D* SD3D12Device::CreateDeviceContext2D()
{
	ComPtr<ID2D1DeviceContext> DeviceContext;
	HR(_interop.Device2D->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &DeviceContext));
	return NewObject<SD2D1DeviceContext>(Factory, this, DeviceContext.Get(), _interop.InteropDevice.Get());
}

IRHITexture2D* SD3D12Device::CreateTexture2D(const RHITexture2DDesc& desc, const RHISubresourceData* initialData)
{
	if (desc.Usage == ERHIBufferUsage::Immutable && initialData == nullptr)
	{
		SE_LOG(LogWindows, Fatal, L"If buffer usage is ERHIBufferUsage::Immutable, you must specify initial data for initialize texture data.");
		return nullptr;
	}

	ComPtr<ID3D12Resource> textureBuf;
	ComPtr<ID3D12Resource> uploadBuf;
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT layout;
	const bool bCreateDynamicBuffer = initialData || desc.Usage == ERHIBufferUsage::Dynamic;

	// Create default texture buffer.
	D3D12_RESOURCE_DESC textureDesc = {};
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDesc.Alignment = 0;
	textureDesc.Width = (UINT64)desc.Width;
	textureDesc.Height = (UINT32)desc.Height;
	textureDesc.DepthOrArraySize = (UINT16)desc.DepthOrArraySize;
	textureDesc.MipLevels = desc.MipLevels;
	textureDesc.Format = (DXGI_FORMAT)desc.Format;
	textureDesc.SampleDesc = (const DXGI_SAMPLE_DESC&)desc.SampleDesc;
	textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	textureDesc.Flags = (D3D12_RESOURCE_FLAGS)desc.Flags;
	D3D12_HEAP_PROPERTIES heapProp = { D3D12_HEAP_TYPE_DEFAULT };

	D3D12_CLEAR_VALUE clearValue;
	if (desc.ClearValue.has_value())
	{
		memcpy(&clearValue, &*desc.ClearValue, sizeof(D3D12_CLEAR_VALUE));
	}

	D3D12_RESOURCE_STATES initialState = initialData ? D3D12_RESOURCE_STATE_COPY_DEST : (D3D12_RESOURCE_STATES)desc.InitialState;
	HR(_device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_SHARED, &textureDesc, initialState, desc.ClearValue.has_value() ? &clearValue : nullptr, IID_PPV_ARGS(&textureBuf)));

	// Create dynamic buffer if need.
	if (bCreateDynamicBuffer)
	{
		UINT64 totalBytes;
		_device->GetCopyableFootprints(&textureDesc, 0, 1, 0, &layout, nullptr, nullptr, &totalBytes);

		D3D12_RESOURCE_DESC bufferDesc = {};
		bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		bufferDesc.Alignment = 0;
		bufferDesc.Width = totalBytes;
		bufferDesc.Height = 1;
		bufferDesc.DepthOrArraySize = 1;
		bufferDesc.MipLevels = 1;
		bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
		bufferDesc.SampleDesc = { 1, 0 };
		bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		bufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		HR(_device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadBuf)));

		if (initialData)
		{
			D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Transition.pResource = textureBuf.Get();
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			barrier.Transition.StateAfter = (D3D12_RESOURCE_STATES)desc.InitialState;
			barrier.Transition.Subresource = 0;

			auto* commandList = NewObject<SD3D12CommandList>(Factory, this);
			commandList->Begin();
			SD3D12Texture2D::UpdateSubresource(commandList, textureBuf.Get(), uploadBuf.Get(), 0, layout, totalBytes, initialData);
			commandList->ResourceBarrier(1, &barrier);
			commandList->End();

			if (desc.Usage != ERHIBufferUsage::Dynamic)
			{
				commandList->PendingGarbageObject(Cast<SObject>(uploadBuf));
				uploadBuf.Reset();
			}

			_immCon->ExecuteCommandList(commandList);
		}
	}

	return NewObject<SD3D12Texture2D>(Factory, this, std::move(textureBuf), std::move(uploadBuf), layout, desc);
}

IRHIBuffer* SD3D12Device::CreateBuffer(const RHIBufferDesc& desc, const RHISubresourceData* initialData)
{
	if (desc.Usage == ERHIBufferUsage::Immutable && initialData == nullptr)
	{
		SE_LOG(LogWindows, Fatal, L"If buffer usage is ERHIBufferUsage::Immutable, you must specify initial data for initialize buffer data.");
		return nullptr;
	}

	ComPtr<ID3D12Resource> buffer;
	ComPtr<ID3D12Resource> uploadBuf;

	// Create default buffer.
	D3D12_RESOURCE_DESC bufferDesc = {};
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Alignment = 0;
	bufferDesc.Width = (UINT64)desc.ByteWidth;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
	bufferDesc.SampleDesc = { 1, 0 };
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	bufferDesc.Flags = (D3D12_RESOURCE_FLAGS)desc.Flags;
	D3D12_HEAP_PROPERTIES heapProp = { D3D12_HEAP_TYPE_DEFAULT };

	D3D12_RESOURCE_STATES initialState = initialData ? D3D12_RESOURCE_STATE_COPY_DEST : (D3D12_RESOURCE_STATES)desc.InitialState;
	HR(_device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, initialState, nullptr, IID_PPV_ARGS(&buffer)));

	// Create dynamic upload buffer if necessary.
	const bool bCreateDynamicBuffer = initialData || desc.Usage == ERHIBufferUsage::Default;
	if (bCreateDynamicBuffer)
	{
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		HR(_device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadBuf)));

		if (initialData)
		{
			D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Transition.pResource = buffer.Get();
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			barrier.Transition.StateAfter = (D3D12_RESOURCE_STATES)desc.InitialState;
			barrier.Transition.Subresource = 0;

			auto* commandList = NewObject<SD3D12CommandList>(Factory, this);
			commandList->Begin();
			SD3D12Buffer::UpdateSubresource(commandList, buffer.Get(), uploadBuf.Get(), 0, (uint64)desc.ByteWidth, initialData);
			commandList->ResourceBarrier(1, &barrier);
			commandList->End();

			if (desc.Usage != ERHIBufferUsage::Dynamic)
			{
				commandList->PendingGarbageObject(Cast<SObject>(uploadBuf));
				uploadBuf.Reset();
			}

			_immCon->ExecuteCommandList(commandList);
		}
	}

	return NewObject<SD3D12Buffer>(Factory, this, std::move(buffer), std::move(uploadBuf), desc);
}

IRHIShader* SD3D12Device::CompileShader(SMaterial* material)
{
	//std::span<uint8 const> vsBytecode = material->CompileVS();
	//std::span<uint8 const> psBytecode = material->CompilePS();

	//std::vector<RHIShaderParameterElement> shaderParameters = material->GetShaderParameterDeclaration();
	//std::vector<D3D12_ROOT_PARAMETER> rootParameters;

	//std::vector<std::vector<D3D12_DESCRIPTOR_RANGE>> rangesCollection;

	//for (size_t i = 0; i < shaderParameters.size(); ++i)
	//{
	//	RHIShaderParameterElement& myParam = shaderParameters[i];
	//	switch (myParam.Type)
	//	{
	//	case ERHIShaderParameterType::ParameterCollection:
	//	case ERHIShaderParameterType::ParameterCollection_CameraConstants:
	//		rootParameters.emplace_back() =
	//		{
	//			.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV,
	//			.Descriptor =
	//			{
	//				.ShaderRegister = myParam.ParameterCollection.ShaderRegister,
	//				.RegisterSpace = myParam.ParameterCollection.RegisterSpace
	//			},
	//			.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL
	//		};
	//		break;
	//	case ERHIShaderParameterType::ScalarParameterConstants:
	//		rootParameters.emplace_back() =
	//		{
	//			.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS,
	//			.Constants =
	//			{
	//				.ShaderRegister = myParam.ScalarConstantsParameter.ShaderRegister,
	//				.RegisterSpace = myParam.ScalarConstantsParameter.RegisterSpace,
	//				.Num32BitValues = myParam.ScalarConstantsParameter.Num32Bits
	//			},
	//			.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL
	//		};
	//		break;
	//	case ERHIShaderParameterType::StructuredBuffer:
	//		rootParameters.emplace_back() =
	//		{
	//			.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV,
	//			.Descriptor =
	//			{
	//				.ShaderRegister = myParam.StructuredBuffer.ShaderRegister,
	//				.RegisterSpace = 0
	//			},
	//			.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL
	//		};
	//		break;
	//	case ERHIShaderParameterType::DescriptorTable:
	//	{
	//		auto& ranges = rangesCollection.emplace_back();

	//		auto& tableRef = myParam.DescriptorTable;
	//		ranges.reserve((size_t)tableRef.NumDescriptorRanges);
	//		for (size_t i = 0; i < tableRef.NumDescriptorRanges; ++i)
	//		{
	//			auto& rangeRef = tableRef.pDescriptorRanges[i];

	//			ranges.emplace_back() =
	//			{
	//				.RangeType = (D3D12_DESCRIPTOR_RANGE_TYPE)rangeRef.RangeType,
	//				.NumDescriptors = rangeRef.NumDescriptors,
	//				.BaseShaderRegister = rangeRef.BaseShaderRegister,
	//				.RegisterSpace = rangeRef.RegisterSpace,
	//				.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
	//			};
	//		}

	//		rootParameters.emplace_back() =
	//		{
	//			.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
	//			.DescriptorTable =
	//			{
	//				.NumDescriptorRanges = tableRef.NumDescriptorRanges,
	//				.pDescriptorRanges = ranges.data()
	//			},
	//			.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL
	//		};
	//		break;
	//	}
	//	default:
	//		SE_LOG(LogWindows, Error, L"Shader parameter type({}) is corrupted.", (int32)myParam.Type);
	//		rootParameters.emplace_back();
	//		break;
	//	}
	//}

	//std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers;
	//staticSamplers.emplace_back() =
	//{
	//	.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR,
	//	.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER,
	//	.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER,
	//	.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER,
	//	.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER,
	//	.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK,
	//	.ShaderRegister = 0,
	//	.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL
	//};
	//staticSamplers.emplace_back() =
	//{
	//	.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT,
	//	.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER,
	//	.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER,
	//	.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER,
	//	.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER,
	//	.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK,
	//	.ShaderRegister = 1,
	//	.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL
	//};

	//D3D12_ROOT_SIGNATURE_DESC rsd =
	//{
	//	.NumParameters = (UINT)rootParameters.size(),
	//	.pParameters = rootParameters.empty() ? nullptr : rootParameters.data(),
	//	.NumStaticSamplers = (UINT)staticSamplers.size(),
	//	.pStaticSamplers = staticSamplers.data(),
	//	.Flags
	//		= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	//		| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS
	//		| D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
	//		| D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS
	//};

	//ComPtr<ID3DBlob> blob, error;
	//HRESULT hr = D3D12SerializeRootSignature(&rsd, D3D_ROOT_SIGNATURE_VERSION_1_0, &blob, &error);
	//if (FAILED(hr))
	//{
	//	// Compile error detected. Print error message and throw fatal exception.
	//	if (error)
	//	{
	//		SE_LOG(LogWindows, Fatal,
	//			L"Could not compile root signature with follow reason:\n{}",
	//			StringUtils::AsUnicode((const char*)error->GetBufferPointer()));
	//	}
	//	else
	//	{
	//		HR(hr);
	//	}
	//}

	//ComPtr<ID3D12RootSignature> rs;
	//HR(_device->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&rs)));

	//// Make vertex declaration to input element.
	//std::vector<RHIVertexElement> declaration;
	//std::vector<D3D12_INPUT_ELEMENT_DESC> inputElements;
	//if (vertexDeclaration)
	//{
	//	declaration = vertexDeclaration->GetVertexDeclaration();
	//	inputElements.resize(declaration.size());

	//	for (size_t i = 0; i < inputElements.size(); ++i)
	//	{
	//		auto& element = declaration[i];

	//		inputElements[i] =
	//		{
	//			.SemanticName = element.SemanticName.c_str(),
	//			.SemanticIndex = element.SemanticIndex,
	//			.Format = (DXGI_FORMAT)element.Format,
	//			.InputSlot = element.InputSlot,
	//			.AlignedByteOffset = element.AlignedByteOffset,
	//			.InputSlotClass = (D3D12_INPUT_CLASSIFICATION)element.InputSlotClass,
	//			.InstanceDataStepRate = 0
	//		};
	//	}
	//}

	//D3D12_GRAPHICS_PIPELINE_STATE_DESC psd =
	//{
	//	.pRootSignature = rs.Get(),
	//	.VS =
	//	{
	//		.pShaderBytecode = vsBytecode.data(),
	//		.BytecodeLength = vsBytecode.size()
	//	},
	//	.PS =
	//	{
	//		.pShaderBytecode = psBytecode.data(),
	//		.BytecodeLength = psBytecode.size()
	//	},
	//	.BlendState =
	//	{
	//		.AlphaToCoverageEnable = FALSE,
	//		.IndependentBlendEnable = FALSE,
	//		.RenderTarget =
	//		{
	//			// [0]
	//			{
	//				.BlendEnable = TRUE,
	//				.LogicOpEnable = FALSE,
	//				.SrcBlend = D3D12_BLEND_SRC_ALPHA,
	//				.DestBlend = D3D12_BLEND_INV_SRC_ALPHA,
	//				.BlendOp = D3D12_BLEND_OP_ADD,
	//				.SrcBlendAlpha = D3D12_BLEND_ONE,
	//				.DestBlendAlpha = D3D12_BLEND_ZERO,
	//				.BlendOpAlpha = D3D12_BLEND_OP_ADD,
	//				.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL
	//			}
	//		}
	//	},
	//	.SampleMask = 0xFFFFFFFF,
	//	.RasterizerState =
	//	{
	//		.FillMode = D3D12_FILL_MODE_SOLID,
	//		.CullMode = D3D12_CULL_MODE_NONE,
	//	},
	//	.DepthStencilState =
	//	{
	//		.DepthEnable = TRUE,
	//		.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL,
	//		.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL,
	//		.StencilEnable = FALSE
	//	},
	//	.InputLayout =
	//	{
	//		.pInputElementDescs = inputElements.empty() ? nullptr : inputElements.data(),
	//		.NumElements = (UINT)inputElements.size()
	//	},
	//	.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
	//	.NumRenderTargets = 1,
	//	.RTVFormats =
	//	{
	//		// [0]
	//		DXGI_FORMAT_B8G8R8A8_UNORM
	//	},
	//	.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT,
	//	.SampleDesc = { 1, 0 },
	//};

	//ComPtr<ID3D12PipelineState> ps;
	//HR(_device->CreateGraphicsPipelineState(&psd, IID_PPV_ARGS(&ps)));

	//return NewObject<SD3D12Shader>(Factory, this, std::move(rs), std::move(ps));
	return nullptr;
}

IRHIRenderTargetView* SD3D12Device::CreateRenderTargetView(int32 count)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = (UINT)count;

	ComPtr<ID3D12DescriptorHeap> heap;
	HR(_device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap)));
	return NewObject<SD3D12RenderTargetView>(Factory, this, std::move(heap), count);
}

IRHIDepthStencilView* SD3D12Device::CreateDepthStencilView(int32 count)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	heapDesc.NumDescriptors = (UINT)count;

	ComPtr<ID3D12DescriptorHeap> heap;
	HR(_device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap)));
	return NewObject<SD3D12DepthStencilView>(Factory, this, std::move(heap), count);
}

IRHIShaderResourceView* SD3D12Device::CreateShaderResourceView(int32 count)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.NumDescriptors = (UINT)count;

	ComPtr<ID3D12DescriptorHeap> heap;
	HR(_device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap)));
	return NewObject<SD3D12ShaderResourceView>(Factory, this, std::move(heap), count);
}

IRHISolidColorBrush* SD3D12Device::CreateSolidColorBrush(const Color& InColor, float InOpacity)
{
	ComPtr<ID2D1SolidColorBrush> Brush;
	HR(_interop.DeviceContext2D->CreateSolidColorBrush((const D2D1_COLOR_F&)InColor, &Brush));
	return NewObject<SD2D1SolidColorBrush>(Factory, this, Brush.Get());
}

IRHIBitmap* SD3D12Device::CreateBitmapFromTexture2D(IRHITexture2D* InTexture)
{
	auto* Texture_s = Cast<SD3D12Texture2D>(InTexture);
	auto* Texture_r = Texture_s->Get<ID3D12Resource>();

	RHITexture2DDesc Desc = Texture_s->GetDesc();

	auto Flags = (D3D12_RESOURCE_FLAGS)Desc.Flags;
	D3D11_RESOURCE_FLAGS BindFlags =
	{
		.BindFlags = GetInteropBindFlag(Flags)
	};

	D3D12_RESOURCE_STATES InteropInitialState = (Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) != 0 ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	ComPtr<ID3D11Resource> WrappedResource;
	HR(_interop.InteropDevice->CreateWrappedResource(Texture_r, &BindFlags, InteropInitialState, (D3D12_RESOURCE_STATES)Desc.InitialState, IID_PPV_ARGS(&WrappedResource)));

	ComPtr<IDXGISurface> Surf;
	HR(WrappedResource.As(&Surf));

	D2D1_BITMAP_OPTIONS Options = D2D1_BITMAP_OPTIONS_NONE;
	if (Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
	{
		Options |= D2D1_BITMAP_OPTIONS_TARGET;
	}

	float DpiScale = IApplicationInterface::Get().GetDpi();
	ComPtr<ID2D1Bitmap1> Bitmap;
	HR(_interop.DeviceContext2D->CreateBitmapFromDxgiSurface(Surf.Get(), D2D1::BitmapProperties1(Options, D2D1::PixelFormat((DXGI_FORMAT)Desc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED), DpiScale, DpiScale), &Bitmap));

	auto* Result = NewObject<SD2D1Bitmap>(Factory, this, Bitmap.Get());
	Result->InitWrappedResources(Texture_r, WrappedResource.Get());

	return Result;
}

void SD3D12Device::BeginFrame()
{
	_immCon->Collect();
	_immCon->WaitCompleted();
}

void SD3D12Device::EndFrame()
{
	_fenceValue = _immCon->ExecuteCommandLists({}, true);
	MarkPendingAllocatorAndHeaps(_fenceValue);
}

void SD3D12Device::FlushCommands()
{
	_interop.DeviceContext->Flush();
}

ID3D12CommandAllocator* SD3D12Device::GetThreadPrimaryAllocator()
{
	auto lock = std::unique_lock(_allocatorLock);
	SD3D12ThreadAllocatorContainer* container = nullptr;

	int64 threadId = Thread::GetCurrentThread()->GetThreadId();
	auto allocator_it = _threadAllocators.find(threadId);
	if (allocator_it == _threadAllocators.end())
	{
		container = NewObject<SD3D12ThreadAllocatorContainer>(threadId, _device.Get());
		_threadAllocators.emplace_hint(allocator_it, threadId, container);
	}
	else
	{
		container = allocator_it->second;
	}

	return container->GetPrimaryAllocator(_immCon->GetCompletedValue());
}

SD3D12DescriptorHeap* SD3D12Device::GetThreadPrimarySrvHeap(int32 count)
{
	auto lock = std::unique_lock(_heapsLock);
	SD3D12ThreadDescriptorHeapContainer* container = nullptr;

	int64 threadId = Thread::GetCurrentThread()->GetThreadId();
	auto heaps_it = _threadSrvHeaps.find(threadId);
	if (heaps_it == _threadSrvHeaps.end())
	{
		container = NewObject<SD3D12ThreadDescriptorHeapContainer>(threadId, _device.Get());
		_threadSrvHeaps.emplace_hint(heaps_it, threadId, container);
	}
	else
	{
		container = heaps_it->second;
	}

	return container->GetUsableHeap(_immCon->GetCompletedValue(), count);
}

SD3D12DescriptorHeap* SD3D12Device::GetThreadPrimarySamplerHeap(int32 count)
{
	auto lock = std::unique_lock(_heapsLock);
	SD3D12ThreadDescriptorHeapContainer* container = nullptr;

	int64 threadId = Thread::GetCurrentThread()->GetThreadId();
	auto heaps_it = _threadSamplerHeaps.find(threadId);
	if (heaps_it == _threadSamplerHeaps.end())
	{
		container = NewObject<SD3D12ThreadDescriptorHeapContainer>(threadId, _device.Get());
		_threadSamplerHeaps.emplace_hint(heaps_it, threadId, container);
	}
	else
	{
		container = heaps_it->second;
	}

	return container->GetUsableHeap(_immCon->GetCompletedValue(), count);
}

void SD3D12Device::MarkPendingAllocatorAndHeaps(uint64 fenceValue)
{
	// Mark fence value as waiting to work completed.
	auto lock1 = std::unique_lock(_allocatorLock);
	auto lock2 = std::unique_lock(_heapsLock);

	for (auto& pair : _threadAllocators)
	{
		pair.second->MarkPendingAllocator(fenceValue);
	}

	for (auto& pair : _threadSrvHeaps)
	{
		pair.second->MarkPendingHeap(fenceValue);
	}

	for (auto& pair : _threadSamplerHeaps)
	{
		pair.second->MarkPendingHeap(fenceValue);
	}
}

void SD3D12Device::AllocateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC desc =
	{
		.Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
		.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL
	};

	ComPtr<ID3D12CommandQueue> queue;
	HR(_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&queue)));
	ComPtr<ID3D12Fence> fence;
	HR(_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

	_immCon = NewObject<SD3D12CommandQueue>(Factory, this, std::move(queue), std::move(fence));
}

void SD3D12Device::CreateInteropDevice()
{
	UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	ID3D12CommandQueue* primaryQueue = _immCon->Get<ID3D12CommandQueue>();
	IUnknown** ppQueues = reinterpret_cast<IUnknown**>(&primaryQueue);
	HR(D3D11On12CreateDevice(_device.Get(), deviceFlags, nullptr, 0, ppQueues, 1, 0, &_interop.Device, &_interop.DeviceContext, nullptr));
	HR(_interop.Device.As(&_interop.InteropDevice));

	ComPtr<IDXGIDevice> dxgiDevice;
	HR(_interop.InteropDevice.As(&dxgiDevice));

	D2D1_DEBUG_LEVEL DebugLevel = D2D1_DEBUG_LEVEL_NONE;
#if defined(_DEBUG)
	DebugLevel = D2D1_DEBUG_LEVEL_WARNING;
#endif

	HR(D2D1CreateDevice(dxgiDevice.Get(), D2D1::CreationProperties(D2D1_THREADING_MODE_MULTI_THREADED, DebugLevel, D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS), &_interop.Device2D));
	HR(_interop.Device2D->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &_interop.DeviceContext2D));
}