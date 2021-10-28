// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SWindow;

class APPENGINE_API SApplicationModule : implements SObject
{
	GENERATED_BODY(SApplicationModule)

public:
	SApplicationModule();

	virtual SWindow* CreateWindow();
};

#define DEFINE_APPLICATION_MODULE(AppModuleClass)			\
extern "C"													\
{															\
	__declspec(dllexport)									\
	SApplicationModule* LoadAppModule(SObject* InOuter)		\
	{														\
		return InOuter->NewObject<AppModuleClass>();		\
	}														\
}