// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ticking/TickScheduler.h"
#include "Misc/TickCalc.h"

interface IApplicationInterface;
class SGameInstance;
class SGameEngineSubsystem;
class SSlateApplication;

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
	virtual bool InitEngine(IApplicationInterface* InApplication);
	virtual bool LoadGameModule(std::wstring_view InModuleName);
	virtual void Shutdown();

	int32 GuardedMain(IApplicationInterface* InApplication, std::wstring_view gameModule);
	SGameInstance* GetGameInstance();
	SSlateApplication* GetSlateApplication();

private:
	std::vector<SGameEngineSubsystem*> Subsystems;
	mutable std::map<size_t, SGameEngineSubsystem*> CachedSubsystems;

	void InitializeSubsystems();

public:
	template<class T>
	T* GetEngineSubsystem()
	{
		size_t Hash = typeid(T).hash_code();
		auto It = CachedSubsystems.find(Hash);
		if (It == CachedSubsystems.end())
		{
			// Find subsystem.
			for (size_t i = 0; i < Subsystems.size(); ++i)
			{
				if (auto ptr = dynamic_cast<T*>(Subsystems[i]); ptr)
				{
					It = CachedSubsystems.emplace(Hash, ptr).first;
					break;
				}
			}

			// Could not found any subsystem class.
			if (It == CachedSubsystems.end())
			{
				return nullptr;
			}
		}

		// Mapping pointer can be cast directly to desired class.
		return static_cast<T*>(It->second);
	}

private:
	void TickEngine();
	TickCalc<> TickCalc;

private:
	void SystemsTick(std::chrono::duration<float> InDeltaTime);
	void GameTick(std::chrono::duration<float> InDeltaTime);
	void RenderTick(std::chrono::duration<float> InDeltaTime);
};

extern GAME_API SGameEngine* GEngine;