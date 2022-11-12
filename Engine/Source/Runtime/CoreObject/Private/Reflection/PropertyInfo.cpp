// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Reflection/PropertyInfo.h"
#include "PropertyInfo.gen.cpp"

PropertyInfo::PropertyInfo(libty::reflect::property_info_t prop)
	: _prop(prop)
{
}

String PropertyInfo::GetName() noexcept
{
	return _prop.name;
}

void* PropertyInfo::GetValue(void* self)
{
	return _prop.getter(self);
}

void PropertyInfo::SetValue(void* self, void* value)
{
	_prop.setter(self, value);
}

Type* PropertyInfo::GetPropertyType() noexcept
{
	return _prop.reflect_type;
}

bool PropertyInfo::IsPublic() noexcept
{
	return _prop.access_modifier == EAccessModifier::Public;
}

bool PropertyInfo::IsProtected() noexcept
{
	return _prop.access_modifier == EAccessModifier::Protected;
}

bool PropertyInfo::IsPrivate() noexcept
{
	return _prop.access_modifier == EAccessModifier::Private;
}