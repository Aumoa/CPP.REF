// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Reflection/Assembly.h"
#include "Reflection/Type.h"
#include "Reflection/TypeInfoMetadataGenerator.Impl.h"
#include "Reflection/FieldInfo.h"
#include "Reflection/FieldInfoMetadataGenerator.Impl.h"

GENERATE_BODY(SAssembly);

SAssembly::SAssembly(std::string_view assemblyName)
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

std::wstring SAssembly::GetName()
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