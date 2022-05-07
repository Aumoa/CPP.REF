// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Reflection/Type.h"
#include "Reflection/FieldInfo.h"
#include "Reflection/MethodInfo.h"
#include "Reflection/TypeInfoMetadataGenerator.Impl.h"
#include "Reflection/Assembly.h"
#include "Attributes/ClassAttribute.h"

using namespace libty;
using namespace libty::Reflection;

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
	// and make full recursive fields and attributes collection.
	for (SType* curr = this; curr != nullptr; curr = curr->_meta.SuperClass)
	{
		_staticCollection->HierarchyView[curr].emplace(this);
		
		auto fields = GetFields(false);
		_recursiveFields.insert(_recursiveFields.end(), fields.begin(), fields.end());

		auto methods = GetMethods(false);
		_recursiveMethods.insert(_recursiveMethods.end(), methods.begin(), methods.end());

		auto attributes = GetCustomAttributes(false);
		_recursiveAttributes.insert(_recursiveAttributes.end(), attributes.begin(), attributes.end());
	}

	if (_meta.Assembly)
	{
		_meta.Assembly->AddType(this);
	}
}

StringView SType::GetName()
{
	return _meta.ClassName;
}

SType* SType::GetSuperType()
{
	return _meta.SuperClass;
}

StringView SType::GetFullQualifiedName()
{
	return _meta.FullQualifiedClassName;
}

std::span<SFieldInfo* const> SType::GetFields(bool bRecursive)
{
	if (bRecursive)
	{
		return _meta.Fields;
	}
	else
	{
		return _recursiveFields;
	}
}

SFieldInfo* SType::GetField(StringView fieldName, bool bRecursive)
{
	auto& collection = bRecursive ? _meta.Fields : _recursiveFields;
	auto it = std::find_if(collection.begin(), collection.end(), [&fieldName](SFieldInfo* field)
	{
		return field->GetName() == fieldName;
	});

	if (it == collection.end())
	{
		return nullptr;
	}

	return *it;
}

std::span<SMethodInfo* const> SType::GetMethods(bool bRecursive)
{
	if (bRecursive)
	{
		return _meta.Methods;
	}
	else
	{
		return _recursiveMethods;
	}
}

SMethodInfo* SType::GetMethod(StringView methodName, bool bRecursive)
{
	auto& collection = bRecursive ? _meta.Methods : _recursiveMethods;
	auto it = std::find_if(collection.begin(), collection.end(), [&methodName](SMethodInfo* method)
	{
		return method->GetName() == methodName;
	});

	if (it == collection.end())
	{
		return nullptr;
	}

	return *it;
}

std::span<SClassAttribute* const> SType::GetCustomAttributes(bool bRecursive)
{
	if (bRecursive)
	{
		return _meta.Attributes;
	}
	else
	{
		return _recursiveAttributes;
	}
}

SClassAttribute* SType::GetCustomAttribute(SType* attributeType, bool bRecursive)
{
	auto& collection = bRecursive ? _meta.Attributes : _recursiveAttributes;
	auto it = std::find_if(collection.begin(), collection.end(), [&attributeType](SClassAttribute* attr)
	{
		return attr->GetType()->IsA(attributeType);
	});

	if (it == collection.end())
	{
		return nullptr;
	}

	return *it;
}

std::span<SType* const> SType::GetInterfaces()
{
	return _meta.Interfaces;
}

SAssembly* SType::GetAssembly()
{
	return _meta.Assembly;
}

size_t SType::GetHashCode()
{
	return _meta.TypeHash;
}

bool SType::IsValueType()
{
	return _meta.bIsValueType;
}

bool SType::IsNativeType()
{
	return _meta.bIsNative;
}

bool SType::IsInterfaceType()
{
	return _meta.bIsInterface;
}

bool SType::IsEnum()
{
	return _meta.bIsEnum;
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

bool SType::IsImplemented(SType* baseTypeOrInterface)
{
	if (baseTypeOrInterface->IsInterfaceType())
	{
		auto pred = [hash = baseTypeOrInterface->GetHashCode()](SType* type)
		{
			return type->GetHashCode() == hash;
		};

		auto it = std::find_if(_meta.Interfaces.begin(), _meta.Interfaces.end(), pred);
		return it != _meta.Interfaces.end();
	}
	else
	{
		return IsDerivedFrom(baseTypeOrInterface);
	}
}

bool SType::IsConstructible()
{
	return (bool)_meta.Constructor;
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

SType* SType::GetType(StringView fullQualifiedName)
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