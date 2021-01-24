// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class CycleStatStorage;

class GAME_API CycleStatsGroup : virtual public Object
{
public:
	using Super = Object;
	using This = CycleStatsGroup;

private:
	static std::vector<CycleStatsGroup*> stats_groups;

	bool bDeferredInit : 1;
	TRefPtr<String> name;
	std::vector<CycleStatStorage*> links;

public:
	CycleStatsGroup(TRefPtr<String> inNamespace);
	~CycleStatsGroup();

	virtual TRefPtr<String> ToString() const override;
	
	void AddLink(CycleStatStorage* inStorage);
	void ResolveLinks();

	static void ResolveFrameDiagnostics();

private:
	void DeferredInit();
};

class GAME_API CycleStatStorage : virtual public Object
{
public:
	using Super = Object;
	using This = CycleStatStorage;

private:
	bool bDeferredInit : 1;
	CycleStatsGroup* myGroup;
	TRefPtr<String> name;

	int64 dur;
	size_t cnt;

	float res[10];
	size_t res_cnt;
	size_t res_idx;
	float resolved;

public:
	CycleStatStorage(CycleStatsGroup* inStatGroup, TRefPtr<String> inName);
	~CycleStatStorage();

	TRefPtr<String> ToString() const override;

	void AddTick(Nanoseconds tick);
	void ResolveTick();

	vs_property_get(bool, HasTick);
	bool HasTick_get() const;
	vs_property_get(Milliseconds, Tick);
	Milliseconds Tick_get() const;

private:
	void DeferredInit();
};

class GAME_API ScopedCycleCounter : virtual public Object
{
public:
	using Super = Object;
	using This = ScopedCycleCounter;
	using MyTimer = std::chrono::steady_clock;

private:
	MyTimer::time_point start;
	MyTimer::duration mydur;

	CycleStatStorage* myStorage;

public:
	ScopedCycleCounter(CycleStatStorage* inStorage);
	~ScopedCycleCounter() override;

	vs_property_get(Seconds, Secs);
	Seconds Secs_get() const;

private:
	void RegisterFrameDiagnostics();
};

#define DECLARE_STATS_GROUP(InNamespace) extern CycleStatsGroup STATGROUP_ ## InNamespace
#define DEFINE_STATS_GROUP(InNamespace) CycleStatsGroup STATGROUP_ ## InNamespace(#InNamespace)

#define DECLARE_CYCLE_STAT(InNamespace, InScopeName) extern CycleStatStorage STAT_InNamespace ## _ ## InScopeName
#define DEFINE_CYCLE_STAT(InNamespace, InScopeName) CycleStatStorage STAT_ ## InScopeName(&STATGROUP_ ## InNamespace, #InScopeName)

#define SCOPED_CYCLE_COUNTER(InScopeName) ScopedCycleCounter CYCLECOUNTER_ ## InScopeName(&STAT_ ## InScopeName)
#define QUICK_SCOPED_CYCLE_COUNTER(InNamespace, InScopeName) static DEFINE_CYCLE_STAT(InNamespace, InScopeName); SCOPED_CYCLE_COUNTER(InScopeName)