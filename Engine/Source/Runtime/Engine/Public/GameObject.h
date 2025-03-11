// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class ENGINE_API GameObject
{
private:
	std::weak_ptr<GameObject> Outer;
	Name ObjectName;

public:
	GameObject();
	
	void Rename(std::shared_ptr<GameObject> InOuter, Name InName);

	inline std::shared_ptr<GameObject> GetOuter() const { return Outer.lock(); }
	inline Name GetName() const { return ObjectName; }
};
