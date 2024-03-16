// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "Object.h"
#include "Reflection/Type.h"
#include "Reflection/Assembly.h"
#include "Reflection/FieldInfo.h"
#include "Reflection/ObjectMacros.h"

ACLASS()
class BLANKAPP_API ABlankAppClass : public AObject
{
	AYLA_DECLARE_STATIC_CLASS_FUNCTION(Engine, BlankApp, BlankAppClass);
	AYLA_DECLARE_CLASS_TYPEDEFS(Engine, BlankApp, BlankAppClass)

public:
	ABlankAppClass()
	{
	}
};

AYLA_DEFINE_CLASS_INFO(Engine, BlankApp, BlankAppClass);
AYLA_DEFINE_STATIC_CLASS_FUNCTION(Engine, BlankApp, BlankAppClass);

int main()
{
	PlatformProcess::SetupStacktraceSignals();
	Console::WriteLine(TEXT("BlankApp"));
	auto obj = AObject::NewObject<ABlankAppClass>();
	AType* type = obj->GetType();
	AAssembly* assembly = type->GetAssembly();
	Console::WriteLine(TEXT("ClassName: {0}"), type->GetName());
	Console::WriteLine(TEXT("AssemblyName: {0}"), assembly->GetFullName());
	AObject::Destroy(obj);

	type = ABlankAppClass::StaticClass();
	while (type != nullptr)
	{
		Console::WriteLine(TEXT("ClassName: {0}"), type->GetName());
		type = type->GetBaseType();
	}

	return 0;
}