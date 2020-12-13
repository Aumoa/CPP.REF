// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "WindowsMinimal.h"

namespace SC::Runtime::Game
{
	class GAME_API Application : virtual public Core::Object
	{
	public:
		using Super = Core::Object;
		using This = Application;

	private:
		static Application* instance;

		HWND hWnd;

	public:
		Application(Core::TRefPtr<Core::String> appName = "GameApp");
		~Application() override;

		static Application* GetInstance();

	private:
		void SetCrtMemLeakDebug();
		void CheckDuplicationAndAlloc();
		void CreateWindow(Core::TRefPtr<Core::String> appName);
	};
}