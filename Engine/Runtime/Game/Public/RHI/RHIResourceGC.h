// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include <vector>

interface IRHIFence;

class GAME_API RHIResourceGC : virtual public Object
{
public:
	using Super = Object;
	using This = RHIResourceGC;

private:
	using Container = std::vector<TRefPtr<Object>>;

	uint64 issued;
	std::vector<Container> issuedContainers;
	uint64 lastCollected;
	std::mutex locker;
	TRefPtr<IRHIFence> fence;

public:
	RHIResourceGC();
	~RHIResourceGC() override;

	uint64 IssueNumber();
	void AddPendingResource(TRefPtr<Object> resource, uint64 number);
	void SignalNumber(uint64 number);
	void Collect();
};