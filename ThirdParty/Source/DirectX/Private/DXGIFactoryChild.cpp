// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DXGIFactoryChild.h"
#include "DXGIFactory.h"

GENERATE_BODY(SDXGIFactoryChild);

SDXGIFactoryChild::SDXGIFactoryChild(SDXGIFactory* InFactory) : Super()
	, Factory(InFactory)
{
}

SDXGIFactoryChild::~SDXGIFactoryChild() noexcept
{
	Dispose(false);
}

void SDXGIFactoryChild::Dispose()
{
	Dispose(true);
}

IRHIFactory* SDXGIFactoryChild::GetFactory()
{
	return Factory;
}

void SDXGIFactoryChild::Dispose(bool bDisposing)
{
	if (bDisposing)
	{
		GC.SuppressFinalize(this);
	}
}