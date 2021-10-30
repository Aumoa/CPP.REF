// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IApplicationInterface;
class SWindow;

class APPENGINE_API SApplicationModule : implements SObject
{
	GENERATED_BODY(SApplicationModule)

private:
	IApplicationInterface* App;

public:
	SApplicationModule();

	virtual void Init(IApplicationInterface* App);
	virtual void Start();

	virtual SWindow* CreateWindow();

	IApplicationInterface* GetApp();
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