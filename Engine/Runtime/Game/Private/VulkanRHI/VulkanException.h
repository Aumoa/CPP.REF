// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "VulkanMinimal.h"

namespace SC::Runtime::Game::VulkanRHI
{
	class VulkanException : public Core::Exception
	{
	public:
		using Super = Exception;
		using This = VulkanException;

	public:
		VulkanException(VkResult vkResult);
		~VulkanException() override;

	private:
		static Core::TRefPtr<Core::String> TranslateMessage(VkResult vkResult);
	};
}