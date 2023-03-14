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

extern "C"
{
	void* EXPORT_CALL CoreAObject__AObject__Construct()
	{
		return (AObject*)new AObject();
	}

	void EXPORT_CALL CoreAObject__AObject__Destruct(void* NativeHandle)
	{
		delete (AObject*)NativeHandle;
	}

	const char_t* EXPORT_CALL CoreAObject__AObject__ToString(void* NativeHandle)
	{
		static thread_local String LocalStringCache = ((AObject*)NativeHandle)->ToString();
		return LocalStringCache.c_str();
	}
}