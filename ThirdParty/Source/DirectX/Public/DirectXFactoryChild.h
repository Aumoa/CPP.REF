// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "RHI/RHIInterfaces.h"
#include "DirectXCommon.h"

class SDirectXFactory;

class SDirectXFactoryChild : implements SObject, implements IRHIFactoryChild
{
	GENERATED_BODY(SDirectXFactoryChild)

private:
	SPROPERTY(Owner)
	IRHIFactory* Owner = nullptr;

public:
	SDirectXFactoryChild(IRHIFactory* Owner);
	SDirectXFactoryChild(SDirectXFactory* Owner);

	virtual void Dispose() override;

	virtual IRHIFactory* GetFactory() override;

protected:
	virtual void Dispose(bool bDisposing);
};