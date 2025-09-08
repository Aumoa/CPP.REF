// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "ScriptingBackend/ScriptingBackend.h"
#include "Platform/DynamicLibrary.h"
#include <memory>

namespace Ayla
{
	class CORE_API CoreCLRScriptingBackend : public ScriptingBackend
	{
		struct Functions;

		std::unique_ptr<DynamicLibrary> m_Hosting;
		std::unique_ptr<Functions> m_Functions;
		void* m_HostHandle = nullptr;
		uint32 m_DomainId = 0;

	public:
		CoreCLRScriptingBackend();
		virtual ~CoreCLRScriptingBackend() noexcept override;

		virtual void LoadAssembly(String assemblyBasePath, String assemblyName) override;
		virtual void UnloadAssembly() override;
		virtual void* GetFunctionPointer(String assemblyName, String className, String methodName) override;
	};
}