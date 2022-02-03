// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VulkanException.h"
#include "RHI/RHIStructures.h"
#include <vulkan/vulkan.hpp>

DECLARE_LOG_CATEGORY(VULKAN_API, LogVulkan);

#define VR(x) \
if (VkResult __VR__ = (x); __VR__ != VK_SUCCESS) \
{ \
	throw gcnew SVulkanException(__VR__, L ## #x, __FILE__, __LINE__); \
}

#define VkCheckSlow(Expr, Code) \
if (bool __Check__ = (bool)(Expr); __Check__ == false) \
{ \
	throw gcnew SVulkanException(Code, L ## #Expr, __FILE__, __LINE__); \
}

inline VkFormat AsVkFormat(ERHIPixelFormat Format)
{
	switch (Format)
	{
	case ERHIPixelFormat::Unknown:
		return VK_FORMAT_UNDEFINED;
	case ERHIPixelFormat::R8G8B8A8_UNORM:
		return VK_FORMAT_R8G8B8A8_UNORM;
	case ERHIPixelFormat::B8G8R8A8_UNORM:
		return VK_FORMAT_B8G8R8A8_UNORM;
	default:
		throw "NOT_IMPLEMENTED";
	}
}

inline VkImageLayout AsVkImageLayout(ERHIResourceStates State)
{
	switch (State)
	{
	case ERHIResourceStates::Present:
		return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	case ERHIResourceStates::RenderTarget:
		return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	default:
		return VK_IMAGE_LAYOUT_UNDEFINED;
	}
}

inline VkImageViewType AsVkImageViewType(ERHIRTVDimension Dimension)
{
	switch (Dimension)
	{
	case ERHIRTVDimension::Buffer:
	case ERHIRTVDimension::Texture1D:
		return VK_IMAGE_VIEW_TYPE_1D;
	case ERHIRTVDimension::Texture1DArray:
		return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
	case ERHIRTVDimension::Texture2D:
	case ERHIRTVDimension::Texture2DMS:
		return VK_IMAGE_VIEW_TYPE_2D;
	case ERHIRTVDimension::Texture2DArray:
	case ERHIRTVDimension::Texture2DMSArray:
		return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
	case ERHIRTVDimension::Texture3D:
		return VK_IMAGE_VIEW_TYPE_3D;
	default:
		// Commonly error enum.
		return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
	}
}

inline VkComponentMapping VK_DEFAULT_COMPONTN_MAPPING()
{
	return
	{
		.r = VK_COMPONENT_SWIZZLE_R,
		.g = VK_COMPONENT_SWIZZLE_G,
		.b = VK_COMPONENT_SWIZZLE_B,
		.a = VK_COMPONENT_SWIZZLE_A
	};
}

inline VkSampleCountFlagBits AsVkSampleCountBits(const RHISampleDesc& SampleDesc)
{
	switch (SampleDesc.Count)
	{
	case 1:
		return VK_SAMPLE_COUNT_1_BIT;
	case 4:
		return VK_SAMPLE_COUNT_4_BIT;
	case 16:
		return VK_SAMPLE_COUNT_16_BIT;
	case 32:
		return VK_SAMPLE_COUNT_32_BIT;
	case 64:
		return VK_SAMPLE_COUNT_64_BIT;
	default:
		return VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
	}
}

inline VkAttachmentLoadOp AsVkAttachmentLoadOp(ERHIAttachmentLoadOp Op)
{
	switch (Op)
	{
	case ERHIAttachmentLoadOp::None:
		return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	case ERHIAttachmentLoadOp::Clear:
		return VK_ATTACHMENT_LOAD_OP_CLEAR;
	case ERHIAttachmentLoadOp::Load:
		return VK_ATTACHMENT_LOAD_OP_LOAD;
	default:
		return VK_ATTACHMENT_LOAD_OP_MAX_ENUM;
	}
}

inline VkAttachmentStoreOp AsVkAttachmentStoreOp(ERHIAttachmentStoreOp Op)
{
	switch (Op)
	{
	case ERHIAttachmentStoreOp::None:
		return VK_ATTACHMENT_STORE_OP_DONT_CARE;
	case ERHIAttachmentStoreOp::Store:
		return VK_ATTACHMENT_STORE_OP_STORE;
	default:
		return VK_ATTACHMENT_STORE_OP_MAX_ENUM;
	}
}