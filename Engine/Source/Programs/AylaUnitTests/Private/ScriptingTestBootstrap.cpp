// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "ScriptingTestBootstrap.h"
#include "Environment.h"

namespace Ayla
{
	ScriptingTestBootstrap::ScriptingTestBootstrap()
		: m_ScriptingBackend(std::make_unique<CoreCLRScriptingBackend>())
	{
		m_ScriptingBackend->LoadAssembly(Environment::GetCurrentDirectory(), TEXT("AylaUnitTests.Script"));
	}

	ScriptingTestBootstrap::~ScriptingTestBootstrap() noexcept
	{
		// Match the normal Launch path: keep CoreCLR loaded until process exit.
	}
}
