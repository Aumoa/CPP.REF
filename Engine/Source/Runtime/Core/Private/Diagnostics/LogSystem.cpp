// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogCategory.h"
#include "Diagnostics/LogVerbosity.h"
#include "Misc/String.h"

using namespace libty;

void LogSystem::InternalLog(LogCategory& Category, ELogVerbosity LogVerbosity, String Message, const std::source_location& Location)
{
	Category.OnLog(LogVerbosity, Message, Location);
}