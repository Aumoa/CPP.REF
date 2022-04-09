// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline PlatformGeneric
{
	struct IRHIFactory;
	struct IPlatformKeyboard;
	struct IPlatformMouse;
	struct IPlatformIME;
	struct IPlatformImageLoader;

	struct PLATFORMGENERIC_API IApplicationInterface : virtual public SObject
	{
		GENERATED_BODY(IApplicationInterface);

	public:
		SENUM(PLATFORMGENERIC_API, ETickMode, int32,
			Realtime,,
			Ontime,
		);

		virtual void Start() = 0;
		virtual void ConsumeModule(std::unique_ptr<PlatformModule> ModulePtr) = 0;

		virtual Vector2N GetViewportSize() = 0;
		virtual float GetDpi() = 0;
		virtual void* GetWindowHandle() = 0;

		virtual void SetTickMode(ETickMode InTickMode) = 0;
		virtual ETickMode GetTickMode() = 0;
		virtual void AddRealtimeDemander(SObject* InObject) = 0;
		virtual void RemoveRealtimeDemander(SObject* InObject) = 0;

		virtual void SetTitle(std::wstring_view InTitle) = 0;
		virtual std::wstring GetTitle() = 0;

		virtual IRHIFactory* GetFactory() = 0;
		virtual IPlatformKeyboard& GetPlatformKeyboard() = 0;
		virtual IPlatformMouse& GetPlatformMouse() = 0;
		virtual IPlatformImageLoader& GetPlatformImageLoader() = 0;
		virtual IPlatformIME& GetPlatformIME() = 0;

		DECLARE_MULTICAST_DELEGATE(IdleDelegate, ETickMode);
		IdleDelegate Idle;
		DECLARE_MULTICAST_DELEGATE(SizedDelegate, Vector2N);
		SizedDelegate Sized;
		DECLARE_MULTICAST_DELEGATE(PreDestroyAppDelegate);
		PreDestroyAppDelegate PreDestroyApp;

		IApplicationInterface();
		static IApplicationInterface& Get();
	};
}