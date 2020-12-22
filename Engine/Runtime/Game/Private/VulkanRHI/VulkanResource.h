// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIResource.h"

#include "VulkanMinimal.h"

namespace SC::Runtime::Game::VulkanRHI
{
	class VulkanResource : virtual public Core::Object, virtual public RHI::IRHIResource
	{
	public:
		using Super = Core::Object;
		using This = VulkanResource;

	private:
		const bool bDynamic : 1;
		VkImage vkImage;

	public:
		VulkanResource(VkImage vkImage, bool bDynamic);
		~VulkanResource() override;

		vs_property_get(VkImage, Image);
		VkImage Image_get() const;
	};
}