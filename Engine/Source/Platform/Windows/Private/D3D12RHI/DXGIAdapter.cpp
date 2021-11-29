// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DXGIAdapter.h"

SDXGIAdapter::SDXGIAdapter(SDXGIFactory* factory, ComPtr<IDXGIAdapter1> adapter) : Super(factory)
	, _adapter(std::move(adapter))
{
}