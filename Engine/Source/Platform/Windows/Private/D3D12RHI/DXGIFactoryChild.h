// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/IRHIFactoryChild.h"

class SDXGIFactory;

class SDXGIFactoryChild : implements SObject, implements IRHIFactoryChild
{
	GENERATED_BODY(SDXGIFactoryChild)

private:
	bool bDisposed = false;

protected:
	SPROPERTY(Factory)
	SDXGIFactory* Factory = nullptr;

public:
	SDXGIFactoryChild(SDXGIFactory* InFactory);
	virtual ~SDXGIFactoryChild() noexcept override;

	virtual void Dispose();
	virtual IRHIFactory* GetFactory() override;

protected:
	virtual void Dispose(bool bDisposing);
};