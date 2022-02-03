// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "PlatformMisc/PlatformMacros.h"
#include "Reflection/ReflectionMacros.h"
#include "Diagnostics/CycleCounterMacros.h"

#define DECLARE_GETTER(Var, ...) __VA_ARGS__ decltype(Var) Get ## Var() const { return this->Var; }
#define DECLARE_SETTER(Var, ...) __VA_ARGS__ void Set ## Var(decltype(Var) New ## Var) { this->Var = New ## Var; }
#define DECLARE_GETTER_SETTER(Var, ...) \
DECLARE_GETTER(Var, __VA_ARGS__)\
DECLARE_SETTER(Var, __VA_ARGS__)
