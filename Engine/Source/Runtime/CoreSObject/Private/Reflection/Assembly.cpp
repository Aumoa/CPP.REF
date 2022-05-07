// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Reflection/Assembly.h"

using namespace libty;
using namespace libty::Reflection;

SAssembly::SAssembly(StringView assemblyName)
	: _assemblyName(String::AsUnicode(assemblyName))
{
}

SAssembly::~SAssembly()
{
	if (_types)
	{
		delete _types;
	}
}

String SAssembly::GetName()
{
	return _assemblyName;
}

std::set<SType*> SAssembly::GetTypes()
{
	return *_types;
}

void SAssembly::AddType(SType* type)
{
	if (_types == nullptr)
	{
		_types = new std::set<SType*>();
	}

	_types->emplace(type);
}