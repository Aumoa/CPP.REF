// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Type.h"
#include "Type.gen.cpp"

Type::Type() noexcept
{
}

Type::~Type() noexcept
{
}

String Type::ToString() const noexcept
{
	return _name;
}

String Type::GetName() const noexcept
{
	return _name;
}