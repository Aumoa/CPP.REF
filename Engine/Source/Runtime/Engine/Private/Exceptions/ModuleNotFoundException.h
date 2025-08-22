// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class ModuleNotFoundException : public Exception
	{
	public:
		explicit ModuleNotFoundException(String moduleName) : Exception(FormatMessage(moduleName))
		{
		}

	private:
		static String FormatMessage(const String& moduleName)
		{
			return String::Format(TEXT("Module not found: {0}"), moduleName);
		}
	};
}