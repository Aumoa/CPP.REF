// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ScriptingBackend/CoreCLR/CoreCLRScriptingBackend.h"
#include <memory>

namespace Ayla
{
	class ScriptingTestBootstrap
	{
	private:
		std::unique_ptr<CoreCLRScriptingBackend> m_ScriptingBackend;

	public:
		ScriptingTestBootstrap();
		~ScriptingTestBootstrap() noexcept;

		ScriptingTestBootstrap(const ScriptingTestBootstrap&) = delete;
		ScriptingTestBootstrap(ScriptingTestBootstrap&&) = delete;
		ScriptingTestBootstrap& operator =(const ScriptingTestBootstrap&) = delete;
		ScriptingTestBootstrap& operator =(ScriptingTestBootstrap&&) = delete;
	};
}
