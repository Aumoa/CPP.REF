// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "WindowsMinimal.h"
#include "IEngineTick.h"

namespace SC::Runtime::Game
{
	namespace Logging
	{
		struct LogCategoryBase;
	}

	class GAME_API Application : virtual public Core::Object
	{
	public:
		using Super = Core::Object;
		using This = Application;

		using PreSizingDelegate = Core::TMulticastDelegate<void(int32, int32)>;
		using PostSizedDelegate = Core::TMulticastDelegate<void(int32, int32)>;

	private:
		static Application* instance;
		static Logging::LogCategoryBase LogApplication;

		HWND hWnd;
		bool bMainLoop : 1;
		Core::TRefPtr<IEngineTick> engineLoop;

	public:
		Application(Core::TRefPtr<Core::String> appName = "GameApp");
		~Application() override;

		virtual void PostInitialize();

		int32 Run();
		HWND GetCoreHwnd() const;

		PreSizingDelegate PreSizing;
		PostSizedDelegate PostSized;

		[[nodiscard]] static Application* GetInstance();

	private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		void SetCrtMemLeakDebug();
		void CheckDuplicationAndAlloc();
		void CreateWindow(Core::TRefPtr<Core::String> appName);
		void InitializeEngine();

		void OnIdle();
	};
}