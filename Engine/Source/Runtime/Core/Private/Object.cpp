// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Object.h"
#include "Type.h"
#include "CoreAssert.h"
#include "GC.h"
#include <list>
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

Object Object::MemberwiseClone()
{
	throw;
}

Type* Object::GenerateClassType(const libty::reflect::ClassTypeMetadata& meta)
{
	static thread_local std::list<std::unique_ptr<Type>> sTypes;
	std::unique_ptr<Type>& ptr = sTypes.emplace_back() = std::unique_ptr<Type>(new Type());
	ptr->GenerateClass(meta);
	return ptr.get();
}