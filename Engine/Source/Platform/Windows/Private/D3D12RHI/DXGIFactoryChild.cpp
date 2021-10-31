// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DXGIFactoryChild.h"
#include "DXGIFactory.h"

SDXGIFactoryChild::SDXGIFactoryChild(SDXGIFactory* factory) : Super()
	, _factory(factory)
{
}

IRHIFactory* SDXGIFactoryChild::GetFactory()
{
	return _factory;
}