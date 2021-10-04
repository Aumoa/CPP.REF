// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/IRHIFactoryChild.h"

class SDXGIFactory;

class SDXGIFactoryChild : implements SObject, implements IRHIFactoryChild
{
	GENERATED_BODY(SDXGIFactoryChild)

protected:
	SDXGIFactory* _factory = nullptr;

public:
	SDXGIFactoryChild(SDXGIFactory* factory);

	virtual IRHIFactory* GetFactory() override;
};