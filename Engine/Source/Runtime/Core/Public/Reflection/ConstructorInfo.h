// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "ConstructorInfo.generated.h"

class Type;

SCLASS()
class CORE_API ConstructorInfo : virtual public Object
{
	GENERATED_BODY()
	friend class Type;

private:
	libty::reflect::constructor_t _ctor;

private:
	ConstructorInfo(libty::reflect::constructor_t ctor);

public:
	template<class... TArgs>
	void* Invoke(TArgs&&... args)
	{
		size_t hash = (0 ^ ... ^ typeid(TArgs).hash_code());
		return InternalInvoke(std::vector<void*>{ ToVoidPtr(std::forward<TArgs>(args), 0)... }, hash);
	}

private:
	void* InternalInvoke(std::vector<void*> args, size_t hash);

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