// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "PlatformMisc/PlatformMacros.h"
#include "Reflection/ReflectionMacros.h"
#include "Diagnostics/CycleCounterMacros.h"

#define interface struct __declspec(novtable)
#define implements virtual public 
#define gcnew SObjectDetails::GCNewBinder() << new