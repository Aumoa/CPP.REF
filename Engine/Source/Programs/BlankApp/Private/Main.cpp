// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "Object.h"
#include "Reflection/Type.h"
#include "Reflection/Assembly.h"
#include "Reflection/FieldInfo.h"

int main()
{
	PlatformProcess::SetupStacktraceSignals();
	Console::WriteLine(TEXT("BlankApp"));
	auto obj = AObject::NewObject<AObject>();
	AType* type = obj->GetType();
	AAssembly* assembly = type->GetAssembly();
	Console::WriteLine(TEXT("ClassName: {0}"), type->GetName());
	Console::WriteLine(TEXT("AssemblyName: {0}"), assembly->GetFullName());
	AObject::Destroy(obj);

	type = AFieldInfo::StaticClass();
	while (type != nullptr)
	{
		Console::WriteLine(TEXT("ClassName: {0}"), type->GetName());
		type = type->GetBaseType();
	}

	return 0;
}