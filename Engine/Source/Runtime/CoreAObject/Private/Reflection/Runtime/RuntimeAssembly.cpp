// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Reflection/Runtime/RuntimeAssembly.h"

ARuntimeAssembly::ARuntimeAssembly(String namespace_, String assemblyName)
	: namespace_(namespace_)
	, assemblyName(assemblyName)
	, fullName(String::Format(TEXT("{0}.{1}"), namespace_, assemblyName))
{
}

String ARuntimeAssembly::GetName() const
{
	return assemblyName;
}

String ARuntimeAssembly::GetNamespace() const
{
	return namespace_;
}

String ARuntimeAssembly::GetFullName() const
{
	return fullName;
}

std::span<AType* const> ARuntimeAssembly::GetTypes() const
{
	return types;
}