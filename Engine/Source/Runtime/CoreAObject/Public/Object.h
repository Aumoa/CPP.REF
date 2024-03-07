// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class AObject;
class ObjectReference;
template<class T>
class TSharedPtr;

extern "C"
{
	PLATFORM_SHARED_EXPORT AObject* AObject__Constructor();
	PLATFORM_SHARED_EXPORT void AObject__Destroy(ObjectReference* reference);
}

class COREAOBJECT_API AObject
{
private:
	ObjectReference* Referencer = nullptr;

public:
	AObject();
	virtual ~AObject() noexcept;
};