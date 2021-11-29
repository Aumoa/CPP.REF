// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DXGIFactoryChild.h"
#include "DirectXCommon.h"
#include "RHI/IRHIAdapter.h"

class SDXGIAdapter : public SDXGIFactoryChild, implements IRHIAdapter
{
	GENERATED_BODY(SDXGIAdapter)

private:
	ComPtr<IDXGIAdapter1> _adapter;

public:
	SDXGIAdapter(SDXGIFactory* factory, ComPtr<IDXGIAdapter1> adapter);

	inline IDXGIAdapter1* GetAdapter() { return _adapter.Get(); }
};