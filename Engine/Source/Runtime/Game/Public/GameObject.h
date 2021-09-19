// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <string>
#include <string_view>

class SWorld;

/// <summary>
/// Represents all classes in the smart component game framework hierarchy and provides low-level services to derived class.
/// </summary>
class GAME_API SGameObject : virtual public SObject
{
	GENERATED_BODY(SGameObject)

private:
	mutable std::wstring _name;

public:
	/// <summary>
	/// Initialize new <see cref="SGameObject"/> instance.
	/// </summary>
	/// <param name="name"> The object name. </param>
	SGameObject();

	/// <inheritdoc/>
	virtual std::wstring ToString() const;

	/// <summary>
	/// Set object name to represents this object.
	/// </summary>
	void SetName(std::wstring_view value) { _name = value; }

	/// <summary>
	/// Get object name to represents this object.
	/// </summary>
	std::wstring GetName() const;

	SWorld* GetWorld() const;
	void SetWorld(SWorld* value);

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