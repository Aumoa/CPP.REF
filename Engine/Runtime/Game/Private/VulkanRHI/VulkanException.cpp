// Copyright 2020 Aumoa.lib. All right reserved.

#include "VulkanException.h"

VulkanException::VulkanException(VkResult vkResult) : Super(TranslateMessage(vkResult))
{

}

VulkanException::~VulkanException()
{

}

TRefPtr<String> VulkanException::TranslateMessage(VkResult vkResult)
{
	TRefPtr<String> errorMessage;

	static auto GetMsg = [](VkResult vkResult)
	{
		switch (vkResult)
		{
		case VK_TIMEOUT:							return L"VK_TIMEOUT: Timeout detected.";
		case VK_ERROR_LAYER_NOT_PRESENT:			return L"VK_ERROR_LAYER_NOT_PRESENT: Layer is not supported.";
		case VK_ERROR_EXTENSION_NOT_PRESENT:		return L"VK_ERROR_EXTENSION_NOT_PRESENT: Extension is not supported.";
		default:									return L"Unexpected error.";
		}
	};

	return String::Format(L"VulkanException: VkResult({0}): {1}", (int32)vkResult, GetMsg(vkResult));
}