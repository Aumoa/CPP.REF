// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "MethodInfo.gen.h"

class Type;

SCLASS()
class CORE_API MethodInfo : virtual public Object
{
	GENERATED_BODY()
	friend class Type;

private:
	libty::reflect::function_info_t _func;

private:
	MethodInfo(libty::reflect::function_info_t func);

public:
	String GetName() noexcept;

	template<class... TArgs>
	void* Invoke(void* self, TArgs&&... args)
	{
		size_t hash = (0 ^ ... ^ typeid(TArgs).hash_code());
		return InternalInvoke(self, std::vector<void*>{ ToVoidPtr(std::forward<TArgs>(args), 0)... }, hash);
	}

	template<class... TArgs>
	void* Invoke(Object* self, TArgs&&... args)
	{
		return Invoke(Cast<void*>(self));
	}

private:
	void* InternalInvoke(void* self, std::vector<void*> args, size_t hash);

	template<class T>
	static void* ToVoidPtr(T&& arg, int) requires
		std::is_pointer_v<std::remove_reference_t<T>> &&
		std::derived_from<std::remove_reference_t<T>, Object>
	{
		return (Object*)arg;
	}

	template<class T>
	static void* ToVoidPtr(T&& arg, short)
	{
		return &arg;
	}
};