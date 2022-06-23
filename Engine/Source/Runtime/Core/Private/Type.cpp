// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Type.h"
#include "Reflection/ConstructorInfo.h"
#include "Type.gen.cpp"

Type::Type() noexcept
{
}

void Type::GenerateClass(const libty::reflect::ClassTypeMetadata& meta)
{
	_name = meta.FriendlyName;
	for (auto& ctor : meta.Constructors)
	{
		_constructors.emplace_back(std::unique_ptr<ConstructorInfo>(new ConstructorInfo(ctor)));
	}
}

Type::~Type() noexcept
{
}

String Type::ToString() noexcept
{
	return _name;
}

String Type::GetName() noexcept
{
	return _name;
}

std::vector<ConstructorInfo*> Type::GetConstructors() noexcept
{
	std::vector<ConstructorInfo*> ctors;
	ctors.reserve(_constructors.size());

	for (auto& ctor : _constructors)
	{
		ctors.emplace_back(ctor.get());
	}

	return ctors;
}