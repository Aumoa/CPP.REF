// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "RHI/IRHIFactory.h"
#include "RHI/DXGI/DXGIFactory.h"

IRHIFactory* IRHIFactory::CreateFactory(SObject* outer)
{
	return outer->NewObject<SDXGIFactory>();
}