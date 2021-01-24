// Copyright 2020 Aumoa.lib. All right reserved.

#include "Diagnostics/ScopedCycleCounter.h"

#include <algorithm>
#include <iomanip>
#include "Logging/LogMacros.h"

using namespace std;

vector<CycleStatsGroup*> CycleStatsGroup::stats_groups;

CycleStatsGroup::CycleStatsGroup(TRefPtr<String> inNamespace) : Super()
	, bDeferredInit(false)
{
	name = inNamespace;
	stats_groups.emplace_back(this);
}

CycleStatsGroup::~CycleStatsGroup()
{

}

TRefPtr<String> CycleStatsGroup::ToString() const
{
	wostringstream woss;

	vector<CycleStatStorage*> active_links;
	active_links.reserve(links.size());

	for (auto& counter : links)
	{
		if (counter->HasTick)
		{
			active_links.emplace_back(counter);
		}
	}

	if (active_links.empty())
	{
		return nullptr;
	}

	// Sort with descending.
	sort(active_links.begin(), active_links.end(), [](const CycleStatStorage* lh, const CycleStatStorage* rh)
	{
		return lh->Tick > rh->Tick;
	});

	woss << name->C_Str;
	for (auto& item : active_links)
	{
		woss << endl << item->ToString()->C_Str;
	}

	return woss.str();
}

void CycleStatsGroup::AddLink(CycleStatStorage* inStorage)
{
	DeferredInit();
	links.emplace_back(inStorage);
}

void CycleStatsGroup::ResolveLinks()
{
	for (auto& item : links)
	{
		item->ResolveTick();
	}
}

void CycleStatsGroup::ResolveFrameDiagnostics()
{
	for (auto& item : stats_groups)
	{
		item->ResolveLinks();
	}
}

void CycleStatsGroup::DeferredInit()
{
	if (!bDeferredInit)
	{
		stats_groups.emplace_back(this);
		bDeferredInit = true;
	}
}

CycleStatStorage::CycleStatStorage(CycleStatsGroup* inStatGroup, TRefPtr<String> inName) : Super()
	, bDeferredInit(false)
	, myGroup(nullptr)

	, dur(0)
	, cnt(0)

	, res{ }
	, res_cnt(0)
	, res_idx(0)
	, resolved(0)
{
	myGroup = inStatGroup;
	name = inName;
}

CycleStatStorage::~CycleStatStorage()
{

}

TRefPtr<String> CycleStatStorage::ToString() const
{
	wostringstream woss;

	woss << L"\t" << setw(40) << left << name->C_Str << setw(10) << right << fixed << setprecision(4) << Milliseconds(dur).Value << L"ms";
	return woss.str();
}

void CycleStatStorage::AddTick(Nanoseconds tick)
{
	DeferredInit();

	dur += tick.Value;
	cnt += 1;
}

void CycleStatStorage::ResolveTick()
{
	float myt;
	
	if (cnt == 0)
	{
		myt = 0;
	}
	else
	{
		myt = Milliseconds(dur).Value / (float)cnt;
		dur = 0;
		cnt = 0;
	}

	// Add tick time to array for calc average.
	if (res_cnt != 10)
	{
		res[res_cnt++] = myt;
	}
	else
	{
		res[res_idx] = myt;
	}

	float avg = 0;

	for (size_t i = 0; i < res_cnt; ++i)
	{
		avg += res[i];
	}

	resolved = avg / (float)res_cnt;
}

bool CycleStatStorage::HasTick_get() const
{
	return resolved != 0;
}

Milliseconds CycleStatStorage::Tick_get() const
{
	return resolved;
}

void CycleStatStorage::DeferredInit()
{
	if (!bDeferredInit)
	{
		if (myGroup == nullptr)
		{
			SE_LOG(LogDiagnostics, Error, L"CycleStatStorage::DeferredInit(): myGroup is invalid.");
			return;
		}

		myGroup->AddLink(this);
		bDeferredInit = true;
	}
}

ScopedCycleCounter::ScopedCycleCounter(CycleStatStorage* inStorage) : Super()
	, myStorage(nullptr)
{
	start = MyTimer::now();
	myStorage = inStorage;
}

ScopedCycleCounter::~ScopedCycleCounter()
{
	auto now = MyTimer::now();
	mydur = now - start;

	RegisterFrameDiagnostics();
}

Seconds ScopedCycleCounter::Secs_get() const
{
	return mydur;
}

void ScopedCycleCounter::RegisterFrameDiagnostics()
{
	if (myStorage == nullptr)
	{
		SE_LOG(LogDiagnostics, Error, L"ScopedCycleCounter::RegisterFrameDiagnostics(): Cycle counter storage is invalid.");
		return;
	}

	myStorage->AddTick(Secs);
}