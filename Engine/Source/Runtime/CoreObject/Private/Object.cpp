// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Object.h"
#include "Type.h"
//#include "CoreAssert.h"
#include "GC.h"
#include "Casts.h"
#include "IDisposable.h"
#include "Reflection/PropertyInfo.h"
#include "Reflection/ConstructorInfo.h"
#include <map>
#include "Object.gen.cpp"

Object::Object() noexcept
{
	Reference = new ObjectReference();
	Reference->IncrWeak();
}

Object::~Object() noexcept
{
	// Mark disposed forcely.
	Reference->bDisposed = true;
	Reference->DecrWeak();
	Reference = nullptr;
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
	Reference->IncrRef();
}

void Object::RemoveFromRoot()
{
	check(InternalIndex != -1);
	Reference->DecrRef();
}

bool Object::IsRoot()
{
	return Reference->Shared > 0;
}

bool Object::IsValidLowLevel()
{
	return GC::IsValidLowLevel(this);
}

AsyncValidator Object::GetValidator()
{
	return AsyncValidator(Reference);
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

std::map<String, std::unique_ptr<Type>>* sTypesPtr;

Type* Object::InstantiateClass(const String& friendlyName)
{
	static std::map<String, std::unique_ptr<Type>> sTypes;
	sTypesPtr = &sTypes;

	auto it = sTypesPtr->find(friendlyName);
	if (it == sTypesPtr->end())
	{
		// Reserve type pointer.
		it = sTypesPtr->emplace(friendlyName, std::unique_ptr<Type>(new Type())).first;
	}

	return it->second.get();
}

Type* Object::GenerateClassType(libty::reflect::ClassTypeMetadata& meta)
{
	if (!meta.bGen)
	{
		meta.bGen = true;

		Type* type = InstantiateClass(meta.FriendlyName);
		// Make class metadata.
		type->GenerateClass(meta);
		return type;
	}
	else
	{
		return FindClass(meta.FriendlyName);
	}
}

Type* Object::FindClass(const String& friendlyName)
{
	if (auto it = sTypesPtr->find(friendlyName); it != sTypesPtr->end())
	{
		return it->second.get();
	}
	else
	{
		return nullptr;
	}
}