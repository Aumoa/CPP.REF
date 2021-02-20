// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "VulkanMinimal.h"

class VulkanException : public Exception
{
public:
	using Super = Exception;
	using This = VulkanException;

public:
	VulkanException(VkResult vkResult);
	~VulkanException() override;

private:
	static TRefPtr<String> TranslateMessage(VkResult vkResult);
};