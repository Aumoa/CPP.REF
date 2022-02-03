// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "VkDeviceChild.h"
#include "VkDevice.h"

GENERATE_BODY(SVkDeviceChild);

SVkDeviceChild::SVkDeviceChild(IRHIDevice* Owner)
	: Super(Owner->GetFactory())
	, Owner(Owner)
{
}

SVkDeviceChild::SVkDeviceChild(SVkDevice* Owner)
	: This((IRHIDevice*)Owner)
{
}

IRHIDevice* SVkDeviceChild::GetDevice()
{
	return Owner;
}

void SVkDeviceChild::Dispose(bool bDisposing)
{
	if (bDisposing)
	{
		Owner = nullptr;
	}

	Super::Dispose(bDisposing);
}