// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXFactoryChild.h"
#include "DirectXFactory.h"

GENERATE_BODY(SDirectXFactoryChild);

SDirectXFactoryChild::SDirectXFactoryChild(IRHIFactory* Owner)
	: Super()
	, Owner(Owner)
{
}

SDirectXFactoryChild::SDirectXFactoryChild(SDirectXFactory* Owner)
	: This((IRHIFactory*)Owner)
{
}

void SDirectXFactoryChild::Dispose()
{
	Dispose(true);
}

IRHIFactory* SDirectXFactoryChild::GetFactory()
{
	return Owner;
}

void SDirectXFactoryChild::Dispose(bool bDisposing)
{
	if (bDisposing)
	{
		Owner = nullptr;
	}
}