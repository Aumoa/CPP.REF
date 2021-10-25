// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Reflection/Type.h"
#include "LogCore.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"

std::unordered_map<size_t, uint64> Type::TypeRegister;
std::mutex Type::TypeRegisterMutex;

const std::wstring& Type::GetFriendlyName() const
{
	return FriendlyName;
}

size_t Type::GetHashCode() const
{
	return TypeHash;
}

Type* Type::GetSuper() const
{
	return SuperClass;
}

SObject* Type::Instantiate(SObject* InOuter) const
{
	if (Constructor)
	{
		return Constructor(InOuter);
	}
	else
	{
		SE_LOG(LogReflection, Fatal, L"Instantiate failed for {0} class. {0} class does not contained constructor without parameters.", FriendlyName);
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
		std::vector<Method> functions = Functions;
		for (const Type* super = this; super; super = super->GetSuper())
		{
			std::vector<Method> superFunctions = super->GetMethods(false);
			functions.insert(functions.end(), superFunctions.begin(), superFunctions.end());
		}
		return functions;
	}
	else
	{
		return Functions;
	}
}

const Method* Type::GetMethod(std::wstring_view friendlyName, bool bIncludeSuperMembers) const
{
	for (size_t i = 0; i < Functions.size(); ++i)
	{
		if (Functions[i].GetFriendlyName() == friendlyName)
		{
			return &Functions[i];
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
		std::vector<Property> properties = Properties;
		for (const Type* super = this; super; super = super->GetSuper())
		{
			std::vector<Property> superProperties = super->GetProperties(false);
			properties.insert(properties.end(), superProperties.begin(), superProperties.end());
		}
		return properties;
	}
	else
	{
		return Properties;
	}
}

const Property* Type::GetProperty(std::wstring_view friendlyName, bool bIncludeSuperMembers) const
{
	for (size_t i = 0; i < Properties.size(); ++i)
	{
		if (Properties[i].GetFriendlyName() == friendlyName)
		{
			return &Properties[i];
		}
	}

	Type* super = nullptr;
	if (!bIncludeSuperMembers || !(super = GetSuper()))
	{
		return nullptr;
	}

	return super->GetProperty(friendlyName, true);
}

std::wstring Type::GenerateUniqueName()
{
	std::unique_lock ScopedLock(TypeRegisterMutex);
	uint64& Incrementer = TypeRegister[TypeHash];
	return std::format(L"{}_{}", GetFriendlyName(), Incrementer++);
}