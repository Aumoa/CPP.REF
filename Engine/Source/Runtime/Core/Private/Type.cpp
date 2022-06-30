// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Type.h"
#include "CoreAssert.h"
#include "Reflection/ConstructorInfo.h"
#include "Reflection/PropertyInfo.h"
#include "Reflection/MethodInfo.h"
#include "Type.gen.cpp"

Type::Type() noexcept
{
}

void Type::GenerateClass(const libty::reflect::ClassTypeMetadata& meta)
{
	_name = meta.FriendlyName;
	_hash_code = meta.HashCode;
	if (meta.Super)
	{
		_base = meta.Super;
	}
	for (auto& i : meta.Interfaces)
	{
		_interfaces.emplace_back(i);
	}
	for (auto& ctor : meta.Constructors)
	{
		_constructors.emplace_back(std::unique_ptr<ConstructorInfo>(new ConstructorInfo(ctor)));
	}
	for (auto& prop : meta.Properties)
	{
		_properties.emplace_back(std::unique_ptr<PropertyInfo>(new PropertyInfo(prop)));
	}
	for (auto& func : meta.Functions)
	{
		_methods.emplace_back(std::unique_ptr<MethodInfo>(new MethodInfo(func)));
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
	_class_type = (EClassType)meta.ClassType;
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

bool Type::IsInterface() noexcept
{
	return _class_type == EClassType::Interface;
}

bool Type::IsDerivedFrom(Type* t) noexcept
{
	check(t && t->IsClass());
	for (Type* p = this; p; p = p->GetBaseType())
	{
		if (p->GetHashCode() == t->GetHashCode())
		{
			return true;
		}
	}
	return false;
}

bool Type::IsImplements(Type* t) noexcept
{
	check(t && t->IsInterface());
	for (Type* p = this; p; p = p->GetBaseType())
	{
		for (auto& i : p->GetInterfaces())
		{
			if (i->GetHashCode() == t->GetHashCode())
			{
				return true;
			}
		}
	}
	return false;
}

size_t Type::GetHashCode() noexcept
{
	return _hash_code;
}

Type* Type::GetBaseType() noexcept
{
	return _base;
}

std::vector<Type*> Type::GetInterfaces() noexcept
{
	return _interfaces;
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

std::vector<MethodInfo*> Type::GetMethods() noexcept
{
	std::vector<MethodInfo*> methods;
	methods.reserve(_methods.size());

	for (auto& method : _methods)
	{
		methods.emplace_back(method.get());
	}

	return methods;
}