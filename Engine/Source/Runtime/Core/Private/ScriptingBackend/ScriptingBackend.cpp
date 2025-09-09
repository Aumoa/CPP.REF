// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "ScriptingBackend/ScriptingBackend.h"
#include "AssertionMacros.h"

namespace Ayla
{
	ScriptingBackend* ScriptingBackend::s_Instance;

	ScriptingBackend::ScriptingBackend()
	{
		check(s_Instance == nullptr);
		s_Instance = this;
	}

	ScriptingBackend::~ScriptingBackend() noexcept
	{
		check(s_Instance == this);
		s_Instance = nullptr;
	}
}