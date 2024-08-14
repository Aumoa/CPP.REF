// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_LINUX

#include "Platform/Clang/ClangPlatformAtomics.h"

struct CORE_API LinuxPlatformAtomics : public ClangPlatformAtomics
{
};

using PlatformAtomics = LinuxPlatformAtomics;

#endif