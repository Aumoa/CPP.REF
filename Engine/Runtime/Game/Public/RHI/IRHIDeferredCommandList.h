// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "IRHICommandList.h"

interface GAME_API IRHIDeferredCommandList : virtual public Object, virtual public IRHICommandList
{

};