// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Game
{
	class SGameInstance;
	class SGameEngineSubsystem;
	//class SSlateApplication;

	class GAME_API SGameEngine : extends(SObject)
	{
		GENERATED_BODY(SGameEngine);

	private:
		SPROPERTY(GameInstance)
		SGameInstance* GameInstance = nullptr;
		//SPROPERTY(SlateApplication)
		//SSlateApplication* SlateApplication = nullptr;

	public:
		SGameEngine();

		virtual bool InitEngine(IApplicationInterface* InApplication);
		virtual bool LoadGameModule(std::wstring_view InModuleName);
		virtual void Shutdown();

		int32 GuardedMain(IApplicationInterface* InApplication, std::wstring_view gameModule);
		SGameInstance* GetGameInstance();
		//SSlateApplication* GetSlateApplication();

	protected:
		//virtual SSlateApplication* CreateSlateApplication();

	private:
		SPROPERTY(Subsystems)
		std::vector<SGameEngineSubsystem*> Subsystems;
		std::map<size_t, SGameEngineSubsystem*> SubsystemView;

		void InitializeSubsystems();

	public:
		template<class T>
		T* GetEngineSubsystem()
		{
			auto It = SubsystemView.find(T::TypeId->GetHashCode());
			if (It == SubsystemView.end())
			{
				return nullptr;
			}
			return Cast<T>(It->second);
		}

	private:
		IApplicationInterface::ETickMode AppTickMode = IApplicationInterface::ETickMode::Realtime;
		::libty::TickCalc<> TickCalc;

		void TickEngine(IApplicationInterface::ETickMode ActualTickMode);

	private:
		void SystemsTick(std::chrono::duration<float> InDeltaTime);
		void GameTick(std::chrono::duration<float> InDeltaTime);
		void RenderTick(std::chrono::duration<float> InDeltaTime);
	};

	extern GAME_API SGameEngine* GEngine;
}