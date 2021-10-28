// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "AppEngine.h"
#include "PlatformCore/IApplicationInterface.h"
#include "RHI/IRHIDevice.h"
#include "Misc/CommandLine.h"
#include "PlatformMisc/PlatformModule.h"
#include "ApplicationModule.h"
#include "Widgets/Window.h"

DEFINE_LOG_CATEGORY(LogEngine);

SAppEngine::SAppEngine() : Super()
{
}

int32 SAppEngine::GuardedMain(IApplicationInterface* Application)
{
	// Load application module.
	SCommandLine CommandLineBuilder(StringUtils::Split(Application->GetCommandLine(), L" ", true, true));
	size_t ModuleArgumentIdx = CommandLineBuilder.GetArgument(L"--Module");
	if (ModuleArgumentIdx == -1)
	{
		SE_LOG(LogEngine, Fatal, L"Could not found --Module argument from command line.");
	}

	std::wstring ApplicationModuleName(*CommandLineBuilder.GetArgument(ModuleArgumentIdx + 1));
	SPlatformModule ApplicationModule(ApplicationModuleName);
	auto Loader = ApplicationModule.GetFunctionPointer<SApplicationModule*(SObject*)>("LoadAppModule");
	if (!Loader)
	{
		SE_LOG(LogEngine, Fatal, L"Module \"{}\" is corrupted. Could not found \"LoadAppModule\" function. Use DEFINE_APP_MODULE macro for define required function.");
	}

	AppModule = Loader(&ApplicationModule);
	if (AppModule == nullptr)
	{
		SE_LOG(LogEngine, Fatal, L"ApplicationModule is nullptr. \"LoadAppModule\" function returns nullptr.");
	}

	CoreWindow = AppModule->CreateWindow();
	CoreWindow->SetOuter(Application);

	// Create graphics resources.
	Device = Application->CreateDevice();
	Device->SetOuter(this);

	Application->Idle.AddSObject(this, &SAppEngine::OnIdle);
	Application->Sized.AddSObject(this, &SAppEngine::OnSized);
	Application->Start();

	Application->DestroyObject(CoreWindow);
	return 0;
}

void SAppEngine::OnIdle()
{
	float DeltaTime = TickCalc.DoCalc().count();

	Device->BeginDraw(NamedColors::White);
	CoreWindow->TickAndPaint(DeltaTime, Device);
	Device->EndDraw();
	Device->Present();
}

void SAppEngine::OnSized(Vector2N Size)
{
	Device->ResizeBuffers(Size);
}