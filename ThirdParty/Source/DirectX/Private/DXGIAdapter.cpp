// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DXGIAdapter.h"

GENERATE_BODY(SDXGIAdapter);

SDXGIAdapter::SDXGIAdapter(SDXGIFactory* InFactory, ComPtr<IDXGIAdapter1> adapter) : Super(InFactory)
	, _adapter(std::move(adapter))
{
}