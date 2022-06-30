// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Object.h"
#include "Type.h"
#include "CoreAssert.h"
#include "GC.h"
#include "Casts.h"
#include "IDisposable.h"
#include "Reflection/PropertyInfo.h"
#include "Reflection/ConstructorInfo.h"
#include <map>
#include "Object.gen.cpp"

Object::Object() noexcept
{
}

Object::~Object() noexcept
{
}

Type* Object::GetType() noexcept
{
	return Impl_GetType();
}

String Object::ToString() noexcept
{
	return GetType()->GetName();
}

void Object::AddToRoot()
{
	check(InternalIndex != -1);
	bRoot = true;
}

void Object::RemoveFromRoot()
{
	check(InternalIndex != -1);
	bRoot = false;
}

bool Object::IsValidLowLevel()
{
	return GC::IsValidLowLevel(this);
}

Object* Object::MemberwiseClone()
{
	for (auto& ctor : GetType()->GetConstructors())
	{
		Object* obj = reinterpret_cast<Object*>(ctor->Invoke());
		
		std::vector<PropertyInfo*> props;
		for (Type* t = GetType(); t; t = t->GetBaseType())
		{
			auto innerProps = t->GetProperties();
			props.insert(props.begin(), innerProps.begin(), innerProps.end());
		}

		for (auto& p : props)
		{
			void* value = p->GetValue(this);
			p->SetValue(obj, value);
		}

		return obj;
	}

	return nullptr;
}

static thread_local std::map<String, std::unique_ptr<Type>> sTypes;

Type* Object::GenerateClassType(const libty::reflect::ClassTypeMetadata& meta)
{
	auto it = sTypes.find(meta.FriendlyName);
	if (it == sTypes.end())
	{
		it = sTypes.emplace(meta.FriendlyName, std::unique_ptr<Type>(new Type())).first;
		it->second->GenerateClass(meta);
	}
	return it->second.get();
}

Type* Object::FindClass(const String& friendlyName)
{
	if (auto it = sTypes.find(friendlyName); it != sTypes.end())
	{
		return it->second.get();
	}
	else
	{
		return nullptr;
	}
}