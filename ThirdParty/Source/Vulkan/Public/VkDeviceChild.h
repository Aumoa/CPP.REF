// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VkFactoryChild.h"

class SVkDevice;

class VULKAN_API SVkDeviceChild : public SVkFactoryChild, implements IRHIDeviceChild
{
	GENERATED_BODY(SVkDeviceChild)

private:
	SPROPERTY(Owner)
	IRHIDevice* Owner = nullptr;

public:
	SVkDeviceChild(IRHIDevice* Owner);
	SVkDeviceChild(SVkDevice* Owner);

	using Super::Dispose;

	virtual IRHIDevice* GetDevice() override;

protected:
	virtual void Dispose(bool bDisposing) override;
};