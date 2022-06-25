// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Reflection/ConstructorInfo.h"
#include "GC.h"
#include "ConstructorInfo.gen.cpp"

ConstructorInfo::ConstructorInfo(libty::reflect::constructor_t ctor)
	: _ctor(ctor)
{
}

void* ConstructorInfo::InternalInvoke(std::vector<void*> args, size_t hash)
{
	void* ptr = _ctor.fnc(std::move(args));
	if (_ctor.is_gc)
	{
		GC::RegisterObject(reinterpret_cast<Object*>(ptr));
	}
	return ptr;
}