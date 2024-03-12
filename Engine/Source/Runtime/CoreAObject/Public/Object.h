// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class AObject;
class ObjectReference;
template<class T>
class TSharedPtr;
class AType;

class COREAOBJECT_API AObject
{
	struct ObjectInitializer;

private:
	AType* classType;
	ObjectReference* referencer = nullptr;

protected:
	AObject();

public:
	virtual ~AObject() noexcept;

	static AObject* NewObject(AType* classType);
	static void Destroy(AObject* instance);
	static AType* StaticClass();

	template<class T>
	static T* NewObject() requires std::derived_from<T, AObject>
	{
		return (T*)NewObject(T::StaticClass());
	}
};