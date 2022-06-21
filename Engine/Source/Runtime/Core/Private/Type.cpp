// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Type.h"
#include "Reflection/ConstructorInfo.h"
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

std::vector<ConstructorInfo*> Type::GetConstructors() const noexcept
{
	std::vector<ConstructorInfo*> ctors;
	ctors.reserve(_constructors.size());
	for (auto& ctor : _constructors)
	{
		ctors.emplace_back(new ConstructorInfo(ctor));
	}
	return ctors;
}