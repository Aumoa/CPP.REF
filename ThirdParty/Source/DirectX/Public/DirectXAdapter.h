// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DirectXFactoryChild.h"

class SDirectXAdapter : public SDirectXFactoryChild, implements IRHIAdapter
{
	GENERATED_BODY(SDirectXAdapter)

public:
	ComPtr<IDXGIAdapter1> pAdapter;

public:
	SDirectXAdapter(SDirectXFactory* Owner, ComPtr<IDXGIAdapter1> pAdapter);

	using Super::Dispose;

	virtual std::wstring GetDeviceName() override;

protected:
	virtual void Dispose(bool bDisposing) override;
};