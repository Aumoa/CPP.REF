// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericSplash.h"

class NBootstrapTask
{
	NBootstrapTask(const NBootstrapTask&) = delete;
	NBootstrapTask(NBootstrapTask&&) = delete;

	struct NContext
	{
		String Name;
		float TotalAmount;
		float CurrentAmount = 0;
	};

	static Spinlock sLock;
	static std::vector<std::unique_ptr<NContext>> sTasks;
	NContext* Context;

public:
	NBootstrapTask(String InName, float Amount);
	~NBootstrapTask() noexcept;

	void Step(float InAmount);

	static void Clear();

private:
	void UpdateContext();
};