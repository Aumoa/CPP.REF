// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Reflection/Type.h"

/// <summary>
/// Gets a type info for specified class.
/// </summary>
#define typeof(x) (SType::TypeOf<x>())