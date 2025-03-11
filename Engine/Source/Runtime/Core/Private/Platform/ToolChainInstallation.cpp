// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/ToolChainInstallation.h"

String ToolChainInstallation::ToString() const
{
    return String::Format(TEXT("{} in {}"), GetInstallVersion().ToString(), GetBaseDirectory());
}