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

size_t Type::GetHashCode() const
{
	return _typeHash;
}

Type* Type::GetSuper() const
{
	return _superClass;
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

bool Type::IsDerivedFrom(const Type* type) const
{
	if (type->GetHashCode() == GetHashCode())
	{
		return true;
	}
	else
	{
		for (const Type* super = this; super; super = super->GetSuper())
		{
			if (type->GetHashCode() == super->GetHashCode())
			{
				return true;
			}
		}

		return false;
	}
}

bool Type::IsBaseOf(const Type* type) const
{
	return type->IsDerivedFrom(this);
}

bool Type::IsA(const Type* type) const
{
	return GetHashCode() == type->GetHashCode();
}

std::vector<Method> Type::GetMethods(bool bIncludeSuperMembers) const
{
	if (bIncludeSuperMembers)
	{
		std::vector<Method> functions = _functions;
		for (const Type* super = this; super; super = super->GetSuper())
		{
			std::vector<Method> superFunctions = super->GetMethods(false);
			functions.insert(functions.end(), superFunctions.begin(), superFunctions.end());
		}
		return functions;
	}
	else
	{
		return _functions;
	}
}

const Method* Type::GetMethod(std::wstring_view friendlyName, bool bIncludeSuperMembers) const
{
	for (size_t i = 0; i < _functions.size(); ++i)
	{
		if (_functions[i].GetFriendlyName() == friendlyName)
		{
			return &_functions[i];
		}
	}

	Type* super = nullptr;
	if (!bIncludeSuperMembers || !(super = GetSuper()))
	{
		return nullptr;
	}

	return super->GetMethod(friendlyName, true);
}

std::vector<Property> Type::GetProperties(bool bIncludeSuperMembers) const
{
	if (bIncludeSuperMembers)
	{
		std::vector<Property> properties = _properties;
		for (const Type* super = this; super; super = super->GetSuper())
		{
			std::vector<Property> superProperties = super->GetProperties(false);
			properties.insert(properties.end(), superProperties.begin(), superProperties.end());
		}
		return properties;
	}
	else
	{
		return _properties;
	}
}

const Property* Type::GetProperty(std::wstring_view friendlyName, bool bIncludeSuperMembers) const
{
	for (size_t i = 0; i < _properties.size(); ++i)
	{
		if (_properties[i].GetFriendlyName() == friendlyName)
		{
			return &_properties[i];
		}
	}

	Type* super = nullptr;
	if (!bIncludeSuperMembers || !(super = GetSuper()))
	{
		return nullptr;
	}

	return super->GetProperty(friendlyName, true);
}