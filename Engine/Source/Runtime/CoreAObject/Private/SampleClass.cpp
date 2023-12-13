// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "SampleClass.h"

ASampleClass::ASampleClass()
{
}

String ASampleClass::ToString()
{
	return String::Format(TEXT("{0}"), value);
}

void ASampleClass::Assign(ASampleClass* instance)
{
	value = instance->value;
}

AType* ASampleClass::StaticClass()
{
	static AType* TypePtr = nullptr;
	if (TypePtr == nullptr)
	{
		GENERATE_INTRINSIC_CLASS_METADATA(SampleClass, EClassMetadata::None);
		NTypeGen::GenerateClassType(TypePtr, Metadata);
	}
	return TypePtr;
}

REGISTER_INTRINSIC_CLASS(ASampleClass, TEXT("/Script/CoreAObject"), ASampleClass::Super::StaticClass());

extern "C"
{
	COREAOBJECT_API void* NativeCall_CoreAObject_SampleClass_Construct()
	{
		return NewObject<ASampleClass>().Drop();
	}

	COREAOBJECT_API void* NativeCall_CoreAObject_SampleClass_StaticClass()
	{
		return (AObject*)ASampleClass::StaticClass();
	}

	COREAOBJECT_API void NativeCall_CoreAObject_SampleClass_Assign(void* __this, void* __arg0)
	{
		return ((ASampleClass*)(AObject*)__this)->Assign((ASampleClass*)(AObject*)__arg0);
	}

	COREAOBJECT_API const char_t* NativeCall_CoreAObject_SampleClass_ToString(void* __this)
	{
		return ((ASampleClass*)(AObject*)__this)->ToString().lock_thread_local();
	}

	COREAOBJECT_API int32 NativeCall_CoreAObject_SampleClass_Get_value(void* __this)
	{
		return ((ASampleClass*)(AObject*)__this)->value;
	}

	COREAOBJECT_API void NativeCall_CoreAObject_SampleClass_Set_value(void* __this, int32 value)
	{
		((ASampleClass*)(AObject*)__this)->value = value;
	}
}