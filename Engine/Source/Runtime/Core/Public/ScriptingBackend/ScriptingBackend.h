// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"
#include "Platform/PlatformMacros.h"

namespace Ayla
{
	class CORE_API ScriptingBackend
	{
	private:
		static ScriptingBackend* s_Instance;

	protected:
		ScriptingBackend();

	public:
		virtual ~ScriptingBackend() noexcept;

		virtual void LoadAssembly(String assemblyBasePath, String assemblyName) = 0;
		virtual void UnloadAssembly() = 0;
		virtual void* GetFunctionPointer(std::string_view assemblyName, std::string_view className, std::string_view methodName) = 0;

		void* GetFunctionPointer(String assemblyName, String className, String methodName)
		{
			auto assemblyName_a = assemblyName.string();
			auto className_a = className.string();
			auto methodName_a = methodName.string();
			return GetFunctionPointer(assemblyName_a, className_a, methodName_a);
		}

		static ScriptingBackend& Get() noexcept { return *s_Instance; }
	};
}