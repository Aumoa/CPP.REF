// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Object.h"

AObject::AObject()
{
	Refs = new Referencer();
	Refs->IncrRef();
}

AObject::~AObject() noexcept
{
	Refs->DecrRef();
}

String AObject::ToString()
{
	return String::FromLiteral(typeid(*this).name());
}

AType* AObject::GetType()
{
	return nullptr;
}