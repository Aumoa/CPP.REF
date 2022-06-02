// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Object.h"
#include "Type.h"

Object::Object() noexcept
{
	using ref = reflexpr(Object);
	using ctors = get_constructors_t<ref>;
	using ctor = get_constructor_t<ctors, 0>;
	invoke_constructor<ctor>(nullptr);
}

Object::~Object() noexcept
{
}

String Object::ToString() const noexcept
{
	return GetType()->GetName();
}