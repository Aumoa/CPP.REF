// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Reflection/MethodInfo.h"
#include "Reflection/MethodInfoMetadataGenerator.Impl.h"
#include "Attributes/MethodAttribute.h"

using namespace libty;
using namespace libty::Reflection;

SMethodInfo::SMethodInfo(MetadataGenerator&& generator)
	: Super()
	, _meta(std::move(generator))
{
}

std::wstring SMethodInfo::GetName()
{
	return _meta.MethodName;
}

bool SMethodInfo::IsStatic()
{
	return _meta.bIsStatic;
}

std::span<SMethodAttribute* const> SMethodInfo::GetAttributes()
{
	return _meta.Attributes;
}

SMethodAttribute* SMethodInfo::GetAttribute(SType* attributeType)
{
	auto it = std::find_if(_meta.Attributes.begin(), _meta.Attributes.end(), [&attributeType](SMethodAttribute* field)
	{
		return field->GetType()->IsA(attributeType);
	});

	if (it == _meta.Attributes.end())
	{
		return nullptr;
	}

	return *it;
}

SType* SMethodInfo::GetReturnType()
{
	return _meta.ReturnType;
}

SObject* SMethodInfo::Invoke(SObject* self, std::span<SObject* const> args)
{
	return _meta.Invoker(self, args);
}