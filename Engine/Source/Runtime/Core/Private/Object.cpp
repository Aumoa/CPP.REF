// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Object.h"
#include "Type.h"
#include <list>

Object::Object() noexcept
{
}

Object::~Object() noexcept
{
}

String Object::ToString() const noexcept
{
	return GetType()->GetName();
}

Object Object::MemberwiseClone() const
{
	throw;
}

Type* Object::GenerateClassType(const libty::reflect::ClassTypeMetadata& meta)
{
	static thread_local std::list<std::unique_ptr<Type>> sTypes;
	std::unique_ptr<Type>& ptr = sTypes.emplace_back() = std::unique_ptr<Type>(new Type());
	ptr->_name = meta.FriendlyName;
	ptr->_constructors = meta.Constructors;
	return ptr.get();
}