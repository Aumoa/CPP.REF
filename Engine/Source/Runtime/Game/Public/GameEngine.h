// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ticking/TickScheduler.h"
#include "Misc/TickCalc.h"

class SGameInstance;
class SGameEngineSubsystem;
class SSlateApplication;
struct IFrameworkView;

/// <summary>
/// Represents game engine that manage core resources.
/// </summary>
class GAME_API SGameEngine : implements SObject
{
	GENERATED_BODY(SGameEngine)

private:
	SGameInstance* GameInstance = nullptr;
	SSlateApplication* SlateApplication = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="SGameEngine"/> instance.
	/// <summary>
	SGameEngine();
	~SGameEngine() override;

	/// <summary>
	/// Initialize engine system.
	/// </summary>
	virtual bool InitEngine();
	virtual void SetupFrameworkView(IFrameworkView* frameworkView);
	virtual bool LoadGameModule(std::wstring_view moduleName);
	virtual void Shutdown();

	int32 InvokedMain(IFrameworkView* frameworkView, std::wstring_view gameModule);
	SGameInstance* GetGameInstance();

private:
	std::vector<SGameEngineSubsystem*> _Subsystems;
	mutable std::map<size_t, SGameEngineSubsystem*> _CachedSubsystemsIndex;

	void InitializeSubsystems();

public:
	template<class T>
	T* GetEngineSubsystem()
	{
		size_t hash = typeid(T).hash_code();
		auto it = _CachedSubsystemsIndex.find(hash);
		if (it == _CachedSubsystemsIndex.end())
		{
			// Find subsystem.
			for (size_t i = 0; i < _Subsystems.size(); ++i)
			{
				if (auto ptr = dynamic_cast<T*>(_Subsystems[i]); ptr)
				{
					it = _CachedSubsystemsIndex.emplace(hash, ptr).first;
					break;
				}
			}

			// Could not found any subsystem class.
			if (it == _CachedSubsystemsIndex.end())
			{
				return nullptr;
			}
		}

		// Mapping pointer can be cast directly to desired class.
		return static_cast<T*>(it->second);
	}

private:
	void TickEngine();
	TickCalc<> _TickCalc;

private:
	void SystemsTick(std::chrono::duration<float> InDeltaTime);
	void GameTick(std::chrono::duration<float> InDeltaTime);
	void RenderTick(std::chrono::duration<float> InDeltaTime);
};

extern GAME_API SGameEngine* GEngine;