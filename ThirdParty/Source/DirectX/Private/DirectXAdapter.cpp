// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXAdapter.h"

using namespace ::libty;

SDirectXAdapter::SDirectXAdapter(SDirectXFactory* Owner, ComPtr<IDXGIAdapter1> pAdapter)
	: Super(Owner)
	, pAdapter(std::move(pAdapter))
{
}

std::wstring SDirectXAdapter::GetDeviceName()
{
	DXGI_ADAPTER_DESC1 Desc1 = {};
	HR(pAdapter->GetDesc1(&Desc1));
	return Desc1.Description;
}

void SDirectXAdapter::Dispose(bool bDisposing)
{
	pAdapter.Reset();
	Super::Dispose(bDisposing);
}