// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"
#include "Object.generated.h"

ACLASS()
class COREAOBJECT_API AObject
{
	GENERATED_BODY()

public:
	AObject();
	virtual ~AObject() noexcept;

	virtual String ToString();
};

//extern "C"
//{
//	COREAOBJECT_API void* EXPORT_CALL CoreAObject__AObject__Construct();
//	COREAOBJECT_API void EXPORT_CALL CoreAObject__AObject__Destruct(void* NativeHandle);
//	COREAOBJECT_API const char_t* EXPORT_CALL CoreAObject__AObject__ToString(void* NativeHandle);
//}