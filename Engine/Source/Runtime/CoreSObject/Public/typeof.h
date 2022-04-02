// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Reflection/Type.h"

/// <summary>
/// Gets a type info for specified class.
/// </summary>
#define typeof(x) (::libty::Core::SType::TypeOf<std::remove_const_t<std::remove_reference_t<x>>>())