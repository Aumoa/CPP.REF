// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "VkFactoryChild.h"
#include "VkFactory.h"

GENERATE_BODY(SVkFactoryChild);

SVkFactoryChild::SVkFactoryChild(IRHIFactory* Owner)
	: Super()
	, Owner(Owner)
{
}

SVkFactoryChild::SVkFactoryChild(SVkFactory* Owner)
	: This((IRHIFactory*)Owner)
{
}

void SVkFactoryChild::Dispose()
{
	Dispose(true);
	GC.SuppressFinalize(this);
}

IRHIFactory* SVkFactoryChild::GetFactory()
{
	return Owner;
}

void SVkFactoryChild::Dispose(bool bDisposing)
{
	if (bDisposing)
	{
		Owner = nullptr;
	}
}