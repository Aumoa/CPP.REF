// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineCore/Engine.h"
#include "EngineCore/GameRenderSubsystem.h"
#include "Reflection/PropertyInfo.h"
#include "Engine.gen.cpp"

Engine::Engine() : Super()
{
}

template<class T> requires std::derived_from<T, EngineSubsystem>
inline T* InstantiateSubsystem()
{
	auto Instance = gcnew T();
	Instance->Init();
	return Instance;
}

void Engine::Init()
{
	Subsystems.emplace_back(InstantiateSubsystem<GameRenderSubsystem>());
}

void Engine::Deinit()
{
	for (auto* Subsystem : Subsystems)
	{
		Subsystem->Deinit();
	}
	Subsystems.clear();
	CachedSubsystemMap.clear();
}

void Engine::Start(GameInstance* Instance)
{
}

void Engine::ExecuteEngineLoop(const TimeSpan& deltaTime)
{
	DispatchEngineTick(deltaTime);
}

EngineSubsystem* Engine::GetEngineSubsystem(Type* SubsystemType)
{
	auto It = CachedSubsystemMap.find(SubsystemType);
	if (It == CachedSubsystemMap.end())
	{
		for (auto& Subsystem : Subsystems)
		{
			if (Subsystem->GetType()->IsDerivedFrom(SubsystemType))
			{
				It = CachedSubsystemMap.emplace(SubsystemType, Subsystem).first;
				break;
			}
		}
	}

	check(It != CachedSubsystemMap.end());
	return It->second;
}

void Engine::DispatchEngineTick(const TimeSpan& deltaTime)
{
}