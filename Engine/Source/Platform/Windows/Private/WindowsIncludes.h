// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "LogWindows.h"

#undef interface
#define NOMINMAX

#include <comdef.h>
#include <Windows.h>
#include <wrl/client.h>

#undef GetCommandLine
using Microsoft::WRL::ComPtr;

#include "WindowsErrors.inl"