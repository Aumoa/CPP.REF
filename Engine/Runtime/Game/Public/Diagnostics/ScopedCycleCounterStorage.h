// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class ScopedCycleCounter;

class GAME_API ScopedCycleCounterStorage : virtual public Object
{
	struct NameStorage
	{
	private:
		float dur;
		size_t cnt;

		float res[10];
		size_t res_cnt;
		size_t res_idx;
		float resolved;

	public:
		NameStorage();
		~NameStorage();

		void AddTick(Seconds sec);
		void ResolveTick();
		
		vs_property_get(Seconds, ResolvedTick);
		Seconds ResolvedTick_get() const;
	};

public:
	using Super = Object;
	using This = ScopedCycleCounterStorage;

private:
	static std::map<TRefPtr<String>, TRefPtr<ScopedCycleCounterStorage>> globalStorages;
	std::map<TRefPtr<String>, NameStorage> tickStorage;
	TRefPtr<String> storageName;

public:
	ScopedCycleCounterStorage(TRefPtr<String> storageName);
	~ScopedCycleCounterStorage() override;

	TRefPtr<String> ToString() const override;

	void AddFrameDiag(ScopedCycleCounter& cycleCounter);
	void ResolveTicks();

	vs_property_get(TRefPtr<String>, StorageName);
	TRefPtr<String> StorageName_get() const;

	static void AddCounter(ScopedCycleCounter& cycleCounter);
	static TRefPtr<String> GetDiagStatics();
	static void MoveFrame();
};