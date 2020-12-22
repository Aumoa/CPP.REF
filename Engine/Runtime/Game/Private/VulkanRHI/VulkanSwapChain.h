﻿// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHISwapChain.h"

#include "VulkanMinimal.h"

namespace SC::Runtime::Game::VulkanRHI
{
	class VulkanResource;

	class VulkanSwapChain : virtual public Core::Object, virtual public RHI::IRHISwapChain
	{
		using Super = Core::Object;
		using This = VulkanSwapChain;

	private:
		VkDevice vkDevice;
		VkSwapchainKHR vkSwapChain;

		VkSwapchainCreateInfoKHR vkCreateInfo;
		VkSurfaceKHR vkSurface;
		size_t bufferCount;

		std::vector<Core::TRefPtr<VulkanResource>> buffers;

	public:
		VulkanSwapChain(VkDevice vkDevice, VkSurfaceKHR vkSurface, int32 width, int32 height);
		~VulkanSwapChain() override;

		virtual void Present();
		virtual void ResizeBuffers(int32 width, int32 height);
		virtual Core::TRefPtr<RHI::IRHIResource> GetBuffer(size_t index) const;

		virtual size_t CurrentBackBufferIndex_get() const;
		virtual size_t NumBuffers_get() const;

	private:
		void ReallocSwapChain();
	};
}