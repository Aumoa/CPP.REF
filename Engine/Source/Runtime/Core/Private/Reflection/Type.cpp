// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Reflection/Type.h"
#include "LogCore.h"
#include "Object.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"

std::unordered_map<size_t, uint64> Type::TypeRegister;
std::mutex Type::TypeRegisterMutex;

std::map<std::wstring, Type*, std::less<>>* TypeCollectionPtr;

void Type::RegisterStaticClass()
{
	static std::map<std::wstring, Type*, std::less<>> TypeCollection;
	TypeCollectionPtr = &TypeCollection;
	TypeCollection.emplace(GetFriendlyName(), this);
}

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
		SObject* SO = Constructor(InOuter);
		return SO;
	}
	else
	{
		SE_LOG(LogReflection, Fatal, L"Instantiate failed for {0} class. {0} class does not contained constructor without parameters.", FriendlyName);
		return nullptr;
	}
}

bool Type::IsDerivedFrom(const Type* InType) const
{
	if (InType->GetHashCode() == GetHashCode())
	{
		return true;
	}
	else
	{
		for (const Type* super = this; super; super = super->GetSuper())
		{
			if (InType->GetHashCode() == super->GetHashCode())
			{
				return true;
			}
		}

		return false;
	}
}

bool Type::IsBaseOf(const Type* InType) const
{
	return InType->IsDerivedFrom(this);
}

bool Type::IsA(const Type* InType) const
{
	return GetHashCode() == InType->GetHashCode();
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

Method* Type::GetMethod(std::wstring_view InFriendlyName, bool bIncludeSuperMembers) const
{
	for (size_t i = 0; i < Functions.size(); ++i)
	{
		if (Functions[i].GetFriendlyName() == InFriendlyName)
		{
			return const_cast<Method*>(&Functions[i]);
		}
	}

	Type* super = nullptr;
	if (!bIncludeSuperMembers || !(super = GetSuper()))
	{
		return nullptr;
	}

	return super->GetMethod(InFriendlyName, true);
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

Property* Type::GetProperty(std::wstring_view InFriendlyName, bool bIncludeSuperMembers) const
{
	for (size_t i = 0; i < Properties.size(); ++i)
	{
		if (Properties[i].GetFriendlyName() == InFriendlyName)
		{
			return const_cast<Property*>(&Properties[i]);
		}
	}

	Type* super = nullptr;
	if (!bIncludeSuperMembers || !(super = GetSuper()))
	{
		return nullptr;
	}

	return super->GetProperty(InFriendlyName, true);
}

std::wstring Type::GenerateUniqueName()
{
	std::unique_lock ScopedLock(TypeRegisterMutex);
	uint64& Incrementer = TypeRegister[TypeHash];
	if (uint64 Value = Incrementer++; Value != 0)
	{
		return std::format(L"{}_{}", GetFriendlyName(), Value);
	}
	else
	{
		return GetFriendlyName();
	}
}

Type* Type::FindStaticClass(std::wstring_view InFriendlyName)
{
	if (auto It = TypeCollectionPtr->find(InFriendlyName); It != TypeCollectionPtr->end())
	{
		return It->second;
	}
	return nullptr;
}