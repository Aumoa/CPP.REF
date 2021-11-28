// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include "SubclassOf.h"

class SLevel;
class SLocalPlayer;
class SGameInstanceSubsystem;

/// <summary>
/// Represents single game instance while application are running.
/// </summary>
class GAME_API SGameInstance : public SGameObject
{
	GENERATED_BODY(SGameInstance)

public:
	SubclassOf<SLevel> StartupLevel;

private:
	std::vector<SGameInstanceSubsystem*> Subsystems;

public:
	/// <summary>
	/// Initialize new <see cref="SGameInstance"/> instance.
	/// </summary>
	SGameInstance();

	/// <summary>
	/// Initialize game instance subsystems.
	/// </summary>
	virtual void Init();

	/// <summary>
	/// Get application name for setting framework title.
	/// </summary>
	virtual std::wstring GetApplicationName();

	/// <summary>
	/// Get local player.
	/// </summary>
	SLocalPlayer* GetLocalPlayer();

	virtual SWorld* GetWorld() override;

	SGameInstanceSubsystem* GetSubsystem(Type* SubsystemClass, bool bAllowDerivedClass = true);
	template<std::derived_from<SGameInstanceSubsystem> T>
	T* GetSubsystem(bool bAllowDerivedClass = true)
	{
		return Cast<T>(GetSubsystem(T::StaticClass()));
	}

private:
	void InitSubsystemCollection();
};