// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystem.h"

class NSubsystemCollection
{
	std::map<AType*, RefPtr<ASubsystem>> Subsystems;

public:
	NSubsystemCollection();
	~NSubsystemCollection() noexcept;

	void Add(AType* InType);

	void Initialize();
	void PostInitialize();
	void Deinitialize();

	ASubsystem* GetSubsystem(AType* InClassType) const;
	template<std::derived_from<ASubsystem> T>
	T* GetSubsystem() const { return static_cast<T*>(GetSubsystem(T::StaticClass())); }

private:
	bool InstantiateAll();
};