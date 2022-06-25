// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Type.h"
#include "Reflection/ConstructorInfo.h"
#include "Reflection/PropertyInfo.h"
#include "Type.gen.cpp"

Type::Type() noexcept
{
}

void Type::GenerateClass(const libty::reflect::ClassTypeMetadata& meta)
{
	_name = meta.FriendlyName;
	if (meta.Super)
	{
		_base = std::unique_ptr<Type>(new Type());
		_base->GenerateClass(*meta.Super);
	}
	for (auto& ctor : meta.Constructors)
	{
		_constructors.emplace_back(std::unique_ptr<ConstructorInfo>(new ConstructorInfo(ctor)));
	}
	for (auto& prop : meta.Properties)
	{
		_properties.emplace_back(std::unique_ptr<PropertyInfo>(new PropertyInfo(prop)));
	}
	for (Type* t = this; t; t = t->GetBaseType())
	{
		for (auto& prop : t->_properties)
		{
			if (Type* pt = prop->GetPropertyType(); pt && pt->IsClass())
			{
				_gcproperties.emplace_back(prop.get());
			}
		}
	}
	_class_type = EClassType::Class;
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

bool Type::IsClass() noexcept
{
	return _class_type == EClassType::Class;
}

Type* Type::GetBaseType() noexcept
{
	return _base.get();
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

std::vector<PropertyInfo*> Type::GetProperties() noexcept
{
	std::vector<PropertyInfo*> props;
	props.reserve(_properties.size());

	for (auto& prop : _properties)
	{
		props.emplace_back(prop.get());
	}

	return props;
}