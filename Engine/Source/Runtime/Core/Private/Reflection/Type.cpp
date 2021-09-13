// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Reflection/Type.h"
#include "LogCore.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"

const std::wstring& Type::GetFriendlyName() const
{
	return _friendlyName;
}

SObject* Type::Instantiate() const
{
	if (_ctor)
	{
		return _ctor();
	}
	else
	{
		SE_LOG(LogReflection, Fatal, L"Instantiate failed for {0} class. {0} class does not contained constructor without parameters.", _friendlyName);
		return nullptr;
	}
}