// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIFactory;

interface IRHIFactoryChild
{
	virtual std::shared_ptr<IRHIFactory> GetFactory() = 0;
};