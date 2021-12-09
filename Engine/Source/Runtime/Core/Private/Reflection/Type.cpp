// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Reflection/Type.h"
#include "LogCore.h"
#include "Object.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"

std::unordered_map<size_t, uint64> Type::TypeRegister;
std::mutex Type::TypeRegisterMutex;

std::map<std::wstring, Type*, std::less<>>* TypeCollectionPtr;
std::map<size_t, std::vector<Type*>>* TypeHierarchyMapPtr;

void Type::RegisterStaticClass()
{
	static std::set<size_t> Collection;
	auto [It, bFlag] = Collection.emplace(GetHashCode());
	if (!bFlag)
	{
		return;
	}

	static std::map<std::wstring, Type*, std::less<>> TypeCollection;
	TypeCollectionPtr = &TypeCollection;
	TypeCollection.emplace(GetFriendlyName(), this);

	static std::map<size_t, std::vector<Type*>> TypeHierarchyMap;
	TypeHierarchyMapPtr = &TypeHierarchyMap;

	for (Type* Curr = this; Curr; Curr = Curr->GetSuper())
	{
		TypeHierarchyMap[Curr->GetHashCode()].emplace_back(this);
	}
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

void Type::CacheProperties()
{
	if (!bPropertyCached)
	{
		CachedProperties.reserve(Properties.size());
		CachedFullProperties.reserve(Properties.size());
		CachedGCProperties.reserve(Properties.size());

		for (auto& Prop : Properties)
		{
			CachedProperties.emplace_back(&Prop);
			CachedFullProperties.emplace_back(&Prop);

			if (!Prop.GetMemberType()->bNative)
			{
				CachedGCProperties.emplace_back(&Prop);
			}
		}

		for (Type* Super = SuperClass; Super; Super = Super->SuperClass)
		{
			CachedFullProperties.reserve(CachedFullProperties.size() + Super->Properties.size());
			CachedGCProperties.reserve(CachedGCProperties.size() + Super->Properties.size());

			for (auto& Prop : Super->Properties)
			{
				CachedFullProperties.emplace_back(&Prop);

				if (!Prop.GetMemberType()->bNative)
				{
					CachedGCProperties.emplace_back(&Prop);
				}
			}
		}

		bPropertyCached = true;
	}
}

std::vector<Method> Type::GetMethods(bool bIncludeSuperMembers)
{
	if (bIncludeSuperMembers)
	{
		std::vector<Method> functions = Functions;
		for (Type* super = this; super; super = super->GetSuper())
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

Method* Type::GetMethod(std::wstring_view InFriendlyName, bool bIncludeSuperMembers)
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

const std::vector<Property*>& Type::GetProperties(bool bIncludeSuperMembers)
{
	CacheProperties();

	if (bIncludeSuperMembers)
	{
		return CachedFullProperties;
	}
	else
	{
		return CachedProperties;
	}
}

const std::vector<Property*>& Type::GetGCProperties()
{
	CacheProperties();
	return CachedGCProperties;
}

Property* Type::GetProperty(std::wstring_view InFriendlyName, bool bIncludeSuperMembers)
{
	CacheProperties();

	auto& List = bIncludeSuperMembers ? CachedFullProperties : CachedProperties;

	for (auto& Prop : List)
	{
		if (Prop->GetFriendlyName() == InFriendlyName)
		{
			return Prop;
		}
	}

	return nullptr;
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

std::span<Type*> Type::FindAllSubclass(Type* BaseClass)
{
	auto It = TypeHierarchyMapPtr->find(BaseClass->GetHashCode());
	if (It != TypeHierarchyMapPtr->end())
	{
		return It->second;
	}

	return std::span<Type*>();
}