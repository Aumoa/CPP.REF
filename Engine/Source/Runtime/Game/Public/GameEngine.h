// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <optional>
#include <chrono>
#include "Ticking/TickScheduler.h"
#include "Misc/TickCalc.h"

class GameInstance;
class ColorVertexFactory;
class ColorShader;
class AssetImporter;
class RHITexture2D;
class TransparentShader;
class SlateShader;
class GameEngineSubsystem;
struct IFrameworkView;

/// <summary>
/// Represents game engine that manage core resources.
/// </summary>
class GAME_API GameEngine : virtual public Object
{
	CLASS_BODY(GameEngine)

private:
	GameInstance* _gameInstance = nullptr;
	std::optional<std::chrono::steady_clock::time_point> _prev;

public:
	/// <summary>
	/// Initialize new <see cref="GameEngine"/> instance.
	/// <summary>
	GameEngine();
	~GameEngine() override;

	/// <summary>
	/// Initialize engine system.
	/// </summary>
	virtual bool InitEngine();
	virtual void SetupFrameworkView(IFrameworkView* frameworkView);
	virtual bool LoadGameModule(std::wstring_view moduleName);

	static int32 InvokedMain(IFrameworkView* frameworkView, std::wstring_view platformArgs);

	GameInstance* GetGameInstance() const;

private:
	std::vector<GameEngineSubsystem*> _subsystems;
	mutable std::map<size_t, GameEngineSubsystem*> _cachedMapSubsystems;

	void InitializeSubsystems();

public:
	template<class T>
	T* GetEngineSubsystem() const
	{
		size_t hash = typeid(T).hash_code();
		auto it = _cachedMapSubsystems.find(hash);
		if (it == _cachedMapSubsystems.end())
		{
			// Find subsystem.
			for (size_t i = 0; i < _subsystems.size(); ++i)
			{
				if (auto ptr = dynamic_cast<T*>(_subsystems[i]); ptr)
				{
					it = _cachedMapSubsystems.emplace(hash, ptr).first;
					break;
				}
			}

			// Could not found any subsystem class.
			if (it == _cachedMapSubsystems.end())
			{
				return nullptr;
			}
		}

		// Mapping pointer can be cast directly to desired class.
		return static_cast<T*>(it->second);
	}

private:
	static GameEngine* _gEngine;

public:
	inline static GameEngine* GetEngine() { return _gEngine; }

private:
	void TickEngine();
	TickCalc<> _tickCalc;

private:
	void SystemsTick(std::chrono::duration<float> elapsedTime);
	void GameTick(std::chrono::duration<float> elapsedTime);
	void RenderTick(std::chrono::duration<float> elapsedTime);
};

#define GEngine (GameEngine::GetEngine())