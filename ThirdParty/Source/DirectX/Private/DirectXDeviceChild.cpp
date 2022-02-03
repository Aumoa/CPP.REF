// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXDeviceChild.h"
#include "DirectXDevice.h"

GENERATE_BODY(SDirectXDeviceChild);

SDirectXDeviceChild::SDirectXDeviceChild(IRHIDevice* Owner)
	: Super(Owner->GetFactory())
	, Owner(Owner)
{
}

SDirectXDeviceChild::SDirectXDeviceChild(SDirectXDevice* Owner)
	: This((IRHIDevice*)Owner)
{
}

IRHIDevice* SDirectXDeviceChild::GetDevice()
{
	return Owner;
}

void SDirectXDeviceChild::Dispose(bool bDisposing)
{
	if (bDisposing)
	{
		Owner = nullptr;
	}

	Super::Dispose(bDisposing);
}