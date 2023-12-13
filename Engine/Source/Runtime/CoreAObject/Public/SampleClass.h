// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "CoreAObject/ClassMetadata.h"

extern "C"
{
	COREAOBJECT_API void* NativeCall_CoreAObject_SampleClass_Construct();
	COREAOBJECT_API void* NativeCall_CoreAObject_SampleClass_StaticClass();
	COREAOBJECT_API void NativeCall_CoreAObject_SampleClass_Assign(void*, void*);
	COREAOBJECT_API const char_t* NativeCall_CoreAObject_SampleClass_ToString(void*);
	COREAOBJECT_API int32 NativeCall_CoreAObject_SampleClass_Get_value(void*);
	COREAOBJECT_API void NativeCall_CoreAObject_SampleClass_Set_value(void*, int32);
}

class COREAOBJECT_API ASampleClass : public AObject
{
	friend struct NTypeGen;
	friend struct NTypeCollection;

	friend COREAOBJECT_API void* NativeCall_CoreAObject_SampleClass_Construct();
	friend COREAOBJECT_API void* NativeCall_CoreAObject_SampleClass_StaticClass();
	friend COREAOBJECT_API void NativeCall_CoreAObject_SampleClass_Assign(void*, void*);
	friend COREAOBJECT_API const char_t* NativeCall_CoreAObject_SampleClass_ToString(void*);
	friend COREAOBJECT_API int32 NativeCall_CoreAObject_SampleClass_Get_value(void*);
	friend COREAOBJECT_API void NativeCall_CoreAObject_SampleClass_Set_value(void*, int32);

public:
	using Super = AObject;

private:
	int32 value = 0;

public:
	ASampleClass();

	virtual String ToString() override;

	void Assign(ASampleClass* instance);

public:
	static AType* StaticClass();
};