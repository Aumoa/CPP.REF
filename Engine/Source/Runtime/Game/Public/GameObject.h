// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <filesystem>

class SLevel;
class SWorld;

/// <summary>
/// Represents all classes in the smart component game framework hierarchy and provides low-level services to derived class.
/// </summary>
class GAME_API SGameObject : implements SObject
{
	GENERATED_BODY(SGameObject)

private:
	SPROPERTY(Outer)
	SGameObject* Outer = nullptr;
	std::wstring Name;

public:
	SGameObject();

	virtual std::wstring ToString() override;

	void SetOuter(SGameObject* NewOuter);
	SGameObject* GetOuter();
	template<std::derived_from<SGameObject> T> T* GetOuter() { return Cast<T>(GetOuter()); }

	void SetName(std::wstring_view InName);
	std::wstring GetName();

	virtual SLevel* GetLevel();
	virtual SWorld* GetWorld();

public:
	SObject* LoadObject(const std::filesystem::path& assetPath);

	template<std::derived_from<SObject> T>
	T* LoadObject(const std::filesystem::path& assetPath)
	{
		return dynamic_cast<T*>(LoadObject(assetPath));
	}

private:
	mutable SWorld* _WorldPrivate = nullptr;
};