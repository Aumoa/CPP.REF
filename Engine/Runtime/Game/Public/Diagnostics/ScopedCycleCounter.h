// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include <string_view>

class CycleStatStorage;

class GAME_API CycleStatsGroup
{
	static CycleStatsGroup* stats_groups[1000];
	static size_t stats_groups_size;

	std::wstring_view name;
	std::vector<CycleStatStorage*> links;

public:
	CycleStatsGroup(std::wstring_view inNamespace);
	~CycleStatsGroup();

	virtual std::wstring ToString() const;
	
	void AddLink(CycleStatStorage* inStorage);
	void ResolveLinks();

	static void ResolveFrameDiagnostics();
	static std::span<CycleStatsGroup*> GetAllStatsGroup();
	static void ReadyToShutdown();
};

class GAME_API CycleStatStorage
{
	bool bDeferredInit : 1;
	CycleStatsGroup* myGroup;
	std::wstring_view name;

	int64 dur;
	size_t cnt;

	float res[10];
	size_t res_cnt;
	size_t res_idx;
	float resolved;

public:
	CycleStatStorage(CycleStatsGroup* inStatGroup, std::wstring_view inName);
	~CycleStatStorage();

	std::wstring ToString() const;

	void AddTick(Nanoseconds tick);
	void ResolveTick();

	vs_property_get(bool, HasTick);
	bool HasTick_get() const;
	vs_property_get(Milliseconds, Tick);
	Milliseconds Tick_get() const;

private:
	void DeferredInit();
};

class GAME_API ScopedCycleCounter
{
public:
	using MyTimer = std::chrono::steady_clock;

private:
	MyTimer::time_point start;
	MyTimer::duration mydur;

	CycleStatStorage* myStorage;

public:
	ScopedCycleCounter(CycleStatStorage* inStorage);
	~ScopedCycleCounter();

	vs_property_get(Seconds, Secs);
	Seconds Secs_get() const;

private:
	void RegisterFrameDiagnostics();
};

#define DECLARE_STATS_GROUP(InNamespace) extern CycleStatsGroup STATGROUP_ ## InNamespace
#define DEFINE_STATS_GROUP(InNamespace) CycleStatsGroup STATGROUP_ ## InNamespace(L ## #InNamespace)

#define DECLARE_CYCLE_STAT(InNamespace, InScopeName) extern CycleStatStorage STAT_InNamespace ## _ ## InScopeName
#define DEFINE_CYCLE_STAT(InNamespace, InScopeName) CycleStatStorage STAT_ ## InScopeName(&STATGROUP_ ## InNamespace, L ## #InScopeName)

#define SCOPED_CYCLE_COUNTER(InScopeName) ScopedCycleCounter CYCLECOUNTER_ ## InScopeName(&STAT_ ## InScopeName)
#define QUICK_SCOPED_CYCLE_COUNTER(InNamespace, InScopeName) static DEFINE_CYCLE_STAT(InNamespace, InScopeName); SCOPED_CYCLE_COUNTER(InScopeName)