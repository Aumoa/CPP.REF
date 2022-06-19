// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Object.h"
#include "Type.h"

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