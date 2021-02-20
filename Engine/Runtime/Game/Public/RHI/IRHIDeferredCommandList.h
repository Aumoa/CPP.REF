// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "IRHICommandList.h"

interface IRHIDeferredCommandList : virtual public Object, virtual public IRHICommandList
{

};