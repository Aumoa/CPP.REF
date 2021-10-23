// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <string>
#include <string_view>

class SLevel;
class SWorld;

/// <summary>
/// Represents all classes in the smart component game framework hierarchy and provides low-level services to derived class.
/// </summary>
class GAME_API SGameObject : implements SObject
{
	GENERATED_BODY(SGameObject)

private:
	std::wstring _Name;

public:
	SGameObject();

	virtual std::wstring ToString(std::wstring_view formatArgs) override;
	void SetName(std::wstring_view InName);
	std::wstring GetName();

	virtual SLevel* GetLevel();
	virtual SWorld* GetWorld();

public:
	static SObject* LoadObject(const std::filesystem::path& assetPath);

	template<std::derived_from<SObject> T>
	static T* LoadObject(const std::filesystem::path& assetPath)
	{
		return dynamic_cast<T*>(LoadObject(assetPath));
	}

private:
	mutable SWorld* _WorldPrivate = nullptr;
};