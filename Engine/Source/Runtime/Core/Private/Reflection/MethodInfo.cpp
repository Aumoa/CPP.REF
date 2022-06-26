// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Reflection/MethodInfo.h"
#include "MethodInfo.gen.cpp"

MethodInfo::MethodInfo(libty::reflect::function_info_t func)
	: _func(func)
{
}

String MethodInfo::GetName() noexcept
{
	return _func.name;
}

void* MethodInfo::InternalInvoke(void* self, std::vector<void*> args, size_t hash)
{
	void* ptr = _func.fnc(self, std::move(args));
	return ptr;
}