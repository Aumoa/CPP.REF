// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "WindowsMinimal.h"
#include "IEngineTick.h"
#include "GameInstance.h"
#include "World.h"

namespace SC::Runtime::Game::Logging
{
	struct LogCategoryBase;
}

namespace SC::Runtime::Game
{
	template<class T>
	concept TIsGameInstance = requires(T* Ref)
	{
		{ static_cast<GameInstance*>(Ref) };
	};

	class GAME_API Application : virtual public Core::Object
	{
	public:
		using Super = Core::Object;
		using This = Application;

		using PreSizingDelegate = Core::TMulticastDelegate<void(int32, int32)>;
		using SizingDelegate = Core::TMulticastDelegate<void(int32, int32)>;
		using PostSizedDelegate = Core::TMulticastDelegate<void(int32, int32)>;

	private:
		static Application* instance;
		static Logging::LogCategoryBase LogApplication;

		HWND hWnd;
		bool bMainLoop : 1;
		Core::TRefPtr<IEngineTick> engineLoop;
		Core::TRefPtr<GameInstance> gameInstance;

	public:
		Application();
		~Application() override;

		virtual void PostInitialize();

		template<TIsGameInstance T, class... TArgs> requires THasConstructor<T, TArgs...>
		inline int32 Run(TArgs&&... args);

		HWND GetCoreHwnd() const;
		GameInstance* GetGameInstance() const;

		PreSizingDelegate PreSizing;
		SizingDelegate Sizing;
		PostSizedDelegate PostSized;

		[[nodiscard]] static Application* GetInstance();

	private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		int32 RunInternal(std::function<Core::TRefPtr<GameInstance>()> objectConstructor);
		void SetCrtMemLeakDebug();
		void CheckDuplicationAndAlloc();
		void CreateWindow();
		void InitializeEngine();

		void OnIdle();
	};
}

#include "Application.inl"

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::Application;
#endif