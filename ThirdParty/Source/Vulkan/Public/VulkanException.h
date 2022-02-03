// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Exceptions/Exception.h"
#include <vulkan/vulkan.hpp>

class VULKAN_API SVulkanException : public SException
{
	GENERATED_BODY(SVulkanException)

public:
	SVulkanException(VkResult Vkr, std::wstring_view Expr, std::string_view File, int32 Line);

public:
	static std::wstring VkResultToString(VkResult Vkr);

private:
	static std::wstring GenerateMessage(VkResult Vkr, std::wstring_view Expr, std::string_view File, int32 Line);
};