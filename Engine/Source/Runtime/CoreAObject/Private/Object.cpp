// Copyright 2020-2022 Aumoa.lib. All right reserved.

import CoreAObject;

AObject::AObject()
{
	Refs = new Referencer();
}

AObject::~AObject() noexcept
{
}

String AObject::ToString()
{
	return String::FromLiteral(typeid(*this).name());
}