// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "SubsystemCollection.h"

NSubsystemCollection::NSubsystemCollection()
{
}

NSubsystemCollection::~NSubsystemCollection() noexcept
{
}

void NSubsystemCollection::Add(AType* InType)
{
	Subsystems.emplace(InType, nullptr);
}

void NSubsystemCollection::Initialize()
{
	while (InstantiateAll());
}

void NSubsystemCollection::PostInitialize()
{
	for (auto& Pair : Subsystems)
	{
		Pair.second->PostInitialize();
	}
}

void NSubsystemCollection::Deinitialize()
{
	for (auto& Pair : Subsystems)
	{
		Pair.second->Deinitialize();
	}
	Subsystems.clear();
}

ASubsystem* NSubsystemCollection::GetSubsystem(AType* InClassType) const
{
	auto It = Subsystems.find(InClassType);
	return It == Subsystems.end() ? nullptr : It->second.Get();
}

bool NSubsystemCollection::InstantiateAll()
{
	bool bInstantiated = false;
	std::vector<AType*> Inits;

	for (auto& Pair : Subsystems)
	{
		if (!Pair.second)
		{
			Inits.emplace_back(Pair.first);
			bInstantiated = true;
		}
	}

	for (auto& Init : Inits)
	{
		RefPtr<ASubsystem>& Subsystem = Subsystems[Init];
		Subsystem = NewObject<ASubsystem>(Init);
		Subsystem->Initialize(*this);
	}

	return bInstantiated;
}