// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIEnums.h"
#include "RHI/RHIStructures.h"

#if PLATFORM_WINDOWS

#include "WindowsRHICommon.h"

inline D3D12_ROOT_PARAMETER_TYPE Convert(ERHIRootParameterType parameterType) noexcept
{
	return (D3D12_ROOT_PARAMETER_TYPE)parameterType;
}

inline D3D12_SHADER_VISIBILITY Convert(ERHIShaderVisibility shaderVisibility) noexcept
{
	return (D3D12_SHADER_VISIBILITY)shaderVisibility;
}

inline D3D12_DESCRIPTOR_RANGE_TYPE Convert(ERHIDescriptorRangeType rangeType) noexcept
{
	return (D3D12_DESCRIPTOR_RANGE_TYPE)rangeType;
}

inline D3D12_FILTER Convert(ERHISamplerFilter filter) noexcept
{
	return (D3D12_FILTER)filter;
}

inline D3D12_TEXTURE_ADDRESS_MODE Convert(ERHITextureAddressMode addressMode) noexcept
{
	return (D3D12_TEXTURE_ADDRESS_MODE)addressMode;
}

inline D3D12_COMPARISON_FUNC Convert(ERHIComparisonFunc comparisonFunc) noexcept
{
	return (D3D12_COMPARISON_FUNC)comparisonFunc;
}

inline D3D12_STATIC_BORDER_COLOR Convert(ERHIStaticBorderColor borderColor) noexcept
{
	return (D3D12_STATIC_BORDER_COLOR)borderColor;
}

inline D3D12_ROOT_SIGNATURE_FLAGS Convert(ERHIRootSignatureFlags flags) noexcept
{
	return (D3D12_ROOT_SIGNATURE_FLAGS)flags;
}

inline D3D12_RESOURCE_DIMENSION Convert(ERHIResourceDimension dimension) noexcept
{
	return (D3D12_RESOURCE_DIMENSION)dimension;
}

inline DXGI_FORMAT Convert(ERHIPixelFormat format) noexcept
{
	return (DXGI_FORMAT)format;
}

inline D3D12_TEXTURE_LAYOUT Convert(ERHITextureLayout layout) noexcept
{
	return (D3D12_TEXTURE_LAYOUT)layout;
}

inline D3D12_RESOURCE_FLAGS Convert(ERHIResourceFlags flags) noexcept
{
	return (D3D12_RESOURCE_FLAGS)flags;
}

inline D3D12_HEAP_TYPE Convert(ERHIHeapType type) noexcept
{
	return (D3D12_HEAP_TYPE)type;
}

inline D3D12_CPU_PAGE_PROPERTY Convert(ERHICPUPageProperty prop) noexcept
{
	return (D3D12_CPU_PAGE_PROPERTY)prop;
}

inline D3D12_MEMORY_POOL Convert(ERHIMemoryPool pool) noexcept
{
	return (D3D12_MEMORY_POOL)pool;
}

inline D3D12_HEAP_FLAGS Convert(ERHIHeapFlags flags) noexcept
{
	return (D3D12_HEAP_FLAGS)flags;
}

inline D3D12_RESOURCE_STATES Convert(ERHIResourceStates state) noexcept
{
	return (D3D12_RESOURCE_STATES)state;
}

struct WindowsConvertMemoryHeap
{
private:
	std::vector<std::vector<uint8>> _blocks;

public:
	template<class TFrom, class TSize>
	auto Alloc(TSize itemCount, const TFrom* from, WindowsConvertMemoryHeap& self)
	{
		using TTo = decltype(Convert(*from, self));

		size_t size = sizeof(TTo) * (size_t)itemCount;
		auto& blob = _blocks.emplace_back(size);
		blob.resize(size);
		TTo* ptr = reinterpret_cast<TTo*>(blob.data());

		for (size_t i = 0; i < (size_t)itemCount; ++i)
		{
			ptr[i] = Convert(from[i], self);
		}

		return ptr;
	}

	template<class TFrom, class TSize>
	auto Alloc(TSize itemCount, const TFrom* from)
	{
		using TTo = decltype(Convert(*from));

		size_t size = sizeof(TTo) * (size_t)itemCount;
		auto& blob = _blocks.emplace_back(size);
		blob.resize(size);
		TTo* ptr = reinterpret_cast<TTo*>(blob.data());

		for (size_t i = 0; i < (size_t)itemCount; ++i)
		{
			ptr[i] = Convert(from[i]);
		}

		return ptr;
	}

	void Clear()
	{
		_blocks.clear();
	}
};

inline D3D12_DESCRIPTOR_RANGE Convert(const RHIDescriptorRange& descriptorRange) noexcept
{
	D3D12_DESCRIPTOR_RANGE output;
	output.RangeType = Convert(descriptorRange.RangeType);
	output.NumDescriptors = descriptorRange.NumDescriptors;
	output.BaseShaderRegister = descriptorRange.BaseShaderRegister;
	output.RegisterSpace = descriptorRange.RegisterSpace;
	output.OffsetInDescriptorsFromTableStart = descriptorRange.OffsetInDescriptorsFromTableStart;
	return output;
}

inline D3D12_ROOT_DESCRIPTOR_TABLE Convert(const RHIRootDescriptorTable& descriptorTable, WindowsConvertMemoryHeap& heap) noexcept
{
	D3D12_ROOT_DESCRIPTOR_TABLE output;
	output.NumDescriptorRanges = descriptorTable.NumDescriptorRanges;
	output.pDescriptorRanges = heap.Alloc(descriptorTable.NumDescriptorRanges, descriptorTable.pDescriptorRanges);
	return output;
}

inline D3D12_ROOT_CONSTANTS Convert(const RHIRootConstants& constants) noexcept
{
	D3D12_ROOT_CONSTANTS output;
	output.ShaderRegister = constants.ShaderRegister;
	output.RegisterSpace = constants.RegisterSpace;
	output.Num32BitValues = constants.Num32BitValues;
	return output;
}

inline D3D12_ROOT_DESCRIPTOR Convert(const RHIRootDescriptor& descriptor) noexcept
{
	D3D12_ROOT_DESCRIPTOR output;
	output.ShaderRegister = descriptor.ShaderRegister;
	output.RegisterSpace = descriptor.RegisterSpace;
	return output;
}

inline D3D12_ROOT_PARAMETER Convert(const RHIRootParameter& parameter, WindowsConvertMemoryHeap& heap) noexcept
{
	D3D12_ROOT_PARAMETER output;
	output.ParameterType = Convert(parameter.ParameterType);
	switch (output.ParameterType)
	{
	case D3D12_ROOT_PARAMETER_TYPE_CBV:
	case D3D12_ROOT_PARAMETER_TYPE_SRV:
	case D3D12_ROOT_PARAMETER_TYPE_UAV:
		output.Descriptor = Convert(parameter.Descriptor);
		break;
	case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
		output.DescriptorTable = Convert(parameter.DescriptorTable, heap);
		break;
	case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS:
		output.Constants = Convert(parameter.Constants);
		break;
	}
	output.ShaderVisibility = Convert(parameter.ShaderVisibility);
	return output;
}

inline D3D12_STATIC_SAMPLER_DESC Convert(const RHIStaticSamplerDesc& sampler) noexcept
{
	D3D12_STATIC_SAMPLER_DESC output;
	output.Filter = Convert(sampler.Filter);
	output.AddressU = Convert(sampler.AddressU);
	output.AddressV = Convert(sampler.AddressV);
	output.AddressW = Convert(sampler.AddressW);
	output.MipLODBias = sampler.MipLODBias;
	output.MaxAnisotropy = sampler.MaxAnisotropy;
	output.ComparisonFunc = Convert(sampler.ComparisonFunc);
	output.BorderColor = Convert(sampler.BorderColor);
	output.MinLOD = sampler.MinLOD;
	output.MaxLOD = sampler.MaxLOD;
	output.ShaderRegister = sampler.ShaderRegister;
	output.RegisterSpace = sampler.RegisterSpace;
	output.ShaderVisibility = Convert(sampler.ShaderVisibility);
	return output;
}

inline D3D12_ROOT_SIGNATURE_DESC Convert(const RHIRootSignatureDesc& desc, WindowsConvertMemoryHeap& heap) noexcept
{
	D3D12_ROOT_SIGNATURE_DESC output;
	output.NumParameters = desc.NumParameters;
	output.pParameters = heap.Alloc(desc.NumParameters, desc.pParameters, heap);
	output.NumStaticSamplers = desc.NumStaticSamplers;
	output.pStaticSamplers = heap.Alloc(desc.NumStaticSamplers, desc.pStaticSamplers);
	output.Flags = Convert(desc.Flags);
	return output;
}

inline DXGI_SAMPLE_DESC Convert(const RHISampleDesc& desc) noexcept
{
	DXGI_SAMPLE_DESC output;
	output.Count = desc.Count;
	output.Quality = desc.Quality;
	return output;
}

inline D3D12_RESOURCE_DESC Convert(const RHIResourceDesc& desc) noexcept
{
	D3D12_RESOURCE_DESC output;
	output.Dimension = Convert(desc.Dimension);
	output.Alignment = desc.Alignment;
	output.Width = desc.Width;
	output.Height = desc.Height;
	output.DepthOrArraySize = desc.DepthOrArraySize;
	output.MipLevels = desc.MipLevels;
	output.Format = Convert(desc.Format);
	output.SampleDesc = Convert(desc.SampleDesc);
	output.Layout = Convert(desc.Layout);
	output.Flags = Convert(desc.Flags);
	return output;
}

inline D3D12_HEAP_PROPERTIES Convert(const RHIHeapProperties& prop) noexcept
{
	D3D12_HEAP_PROPERTIES output;
	output.Type = Convert(prop.Type);
	output.CPUPageProperty = Convert(prop.CPUPageProperty);
	output.MemoryPoolPreference = Convert(prop.MemoryPoolPreference);
	output.CreationNodeMask = prop.CreationNodeMask;
	output.VisibleNodeMask = prop.VisibleNodeMask;
	return output;
}

inline D3D12_CLEAR_VALUE Convert(const RHIClearValue& clear) noexcept
{
	D3D12_CLEAR_VALUE output;
	output.Format = Convert(clear.Format);
	memcpy(output.Color, clear.Color, sizeof(float) * 4);
	return output;
}

#endif