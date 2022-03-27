// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Reflection/Type.h"
#include "Reflection/FieldInfo.h"
#include "Reflection/TypeInfoMetadataGenerator.Impl.h"

GENERATE_BODY(SType);

SType::StaticCollection* SType::_staticCollection;

SType::SType(MetadataGenerator&& generator)
	: Super()
	, _meta(std::move(generator))
{
	static StaticCollection __STATIC_COLLECTION__;
	_staticCollection = &__STATIC_COLLECTION__;

	// Add to dictionary for search by name.
	_staticCollection->FullQualifiedNameView.emplace(_meta.FullQualifiedClassName, this);
	
	// Add to dictionary for search by hierarchy.
	for (SType* curr = this; curr != nullptr; curr = curr->_meta.SuperClass)
	{
		_staticCollection->HierarchyView[curr].emplace(this);
	}
}

std::wstring_view SType::GetName()
{
	return _meta.ClassName;
}

SType* SType::GetSuperType()
{
	return _meta.SuperClass;
}

std::wstring_view SType::GetFullQualifiedName()
{
	return _meta.FullQualifiedClassName;
}

std::span<SFieldInfo* const> SType::GetFields()
{
	return _meta.Fields;
}

SFieldInfo* SType::GetField(std::wstring_view fieldName)
{
	auto it = std::find_if(_meta.Fields.begin(), _meta.Fields.end(), [&fieldName](SFieldInfo* field)
	{
		return field->GetName() == fieldName;
	});

	if (it == _meta.Fields.end())
	{
		return nullptr;
	}

	return *it;
}

size_t SType::GetHashCode()
{
	return _meta.TypeHash;
}

bool SType::IsValueType()
{
	return !_meta.bIsClass;
}

bool SType::IsNativeType()
{
	return _meta.bIsNative;
}

bool SType::IsA(SType* compareType)
{
	return _meta.TypeHash == compareType->_meta.TypeHash;
}

bool SType::IsDerivedFrom(SType* compareType)
{
	for (SType* curr = this; curr != nullptr; curr = curr->_meta.SuperClass)
	{
		if (curr->_meta.TypeHash == compareType->_meta.TypeHash)
		{
			return true;
		}
	}

	return false;
}

SObject* SType::Instantiate()
{
	if (_meta.Constructor)
	{
		return _meta.Constructor();
	}
	else
	{
		return nullptr;
	}
}

SType* SType::GetType(std::wstring_view fullQualifiedName)
{
	auto it = _staticCollection->FullQualifiedNameView.find(fullQualifiedName);
	if (it == _staticCollection->FullQualifiedNameView.end())
	{
		return nullptr;
	}
	return it->second;
}

std::set<SType*> SType::GetDerivedTypes(SType* baseType)
{
	auto it = _staticCollection->HierarchyView.find(baseType);
	if (it == _staticCollection->HierarchyView.end())
	{
		return {};
	}
	return it->second;
}