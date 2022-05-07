// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Reflection/FieldInfo.h"
#include "Reflection/FieldInfoMetadataGenerator.Impl.h"
#include "Reflection/Type.h"
#include "Attributes/FieldAttribute.h"

using namespace libty;
using namespace libty::Reflection;

SFieldInfo::SFieldInfo(MetadataGenerator&& generator)
	: Super()
	, _meta(std::move(generator))
{
}

String SFieldInfo::GetName()
{
	return _meta.FieldName;
}

bool SFieldInfo::IsReadOnly()
{
	return _meta.bIsConst;
}

bool SFieldInfo::IsStatic()
{
	return _meta.bIsStatic;
}

void SFieldInfo::SetValue(SObject* obj, SObject* value)
{
	if (IsReadOnly())
	{
		throw InvalidOperationException("The specified field is readonly.");
	}

	if (!IsStatic() and obj == nullptr)
	{
		throw InvalidOperationException("Field is non-static member but this pointer is not specified.");
	}

	_meta.Setter(obj, value);
}

SObject* SFieldInfo::GetValue(SObject* obj)
{
	if (!IsStatic() and obj == nullptr)
	{
		throw InvalidOperationException("Field is non-static member but this pointer is not specified.");
	}

	return _meta.Getter(obj);
}

std::span<SFieldAttribute* const> SFieldInfo::GetAttributes()
{
	return _meta.Attributes;
}

SFieldAttribute* SFieldInfo::GetAttribute(SType* attributeType)
{
	auto it = std::find_if(_meta.Attributes.begin(), _meta.Attributes.end(), [&attributeType](SFieldAttribute* field)
	{
		return field->GetType()->IsA(attributeType);
	});

	if (it == _meta.Attributes.end())
	{
		return nullptr;
	}

	return *it;
}

SType* SFieldInfo::GetFieldType()
{
	return _meta.FieldType;
}