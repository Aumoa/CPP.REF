// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"
#include "Platform/PlatformMacros.h"

namespace Ayla
{
	class CORE_API ScriptingBackend
	{
	protected:
		ScriptingBackend();

	public:
		virtual ~ScriptingBackend() noexcept;

		virtual void LoadAssembly(String assemblyBasePath, String assemblyName) = 0;
		virtual void UnloadAssembly() = 0;
		virtual void* GetFunctionPointer(String assemblyName, String className, String methodName) = 0;
	};
}