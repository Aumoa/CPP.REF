// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Object.h"

AObject::AObject()
{
}

AObject::~AObject() noexcept
{
}

String AObject::ToString()
{
	return String::FromLiteral(typeid(*this).name());
}