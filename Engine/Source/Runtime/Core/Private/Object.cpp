// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Object.h"
#include "Type.h"
#include <list>
#include "Object.gen.cpp"

Object::Object() noexcept
{
}

Object::Object(int32 number) noexcept
{
	static int32 value = number;
}

Object::Object(Object* object) noexcept
{
	static Object* value = object;
}

Object::Object(const String& str) noexcept
{
	static const String& value = str;
}

Object::~Object() noexcept
{
}

Type* Object::GetType() const noexcept
{
	return Impl_GetType();
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