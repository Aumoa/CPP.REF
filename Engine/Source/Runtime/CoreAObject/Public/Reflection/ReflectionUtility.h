// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class AObject;
class AType;

struct COREAOBJECT_API ReflectionUtility
{
	template<class T>
	static AObject*(*GetConstructorFunction())() requires std::constructible_from<T>
	{
		return +[]() -> AObject* { return new T(); };
	}

	template<class T>
	static AObject*(*GetConstructorFunction())() requires (!std::constructible_from<T>)
	{
		return nullptr;
	}

	static void InjectTypeInfo(AObject* builtInInstance, AType* builtInType);
};