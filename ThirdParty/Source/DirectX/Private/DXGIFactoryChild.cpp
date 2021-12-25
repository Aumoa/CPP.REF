// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DXGIFactoryChild.h"
#include "DXGIFactory.h"
#include "Exceptions/ObjectDisposedException.h"

GENERATE_BODY(SDXGIFactoryChild);

SDXGIFactoryChild::SDXGIFactoryChild(SDXGIFactory* InFactory) : Super()
	, Factory(InFactory)
{
}

void SDXGIFactoryChild::Dispose()
{
	if (bDisposed)
	{
		throw gcnew SObjectDisposedException(GetType()->GetFullName());
	}

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

	bDisposed = true;
}