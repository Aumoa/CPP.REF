// Copyright 2020 Aumoa.lib. All right reserved.

#include "Diagnostics/ScopedCycleCounterStorage.h"

#include <algorithm>
#include "Diagnostics/ScopedCycleCounter.h"

using namespace std;

map<TRefPtr<String>, TRefPtr<ScopedCycleCounterStorage>> ScopedCycleCounterStorage::globalStorages;

ScopedCycleCounterStorage::NameStorage::NameStorage()
	: dur(0)
	, cnt(0)
	, res{ }
	, res_cnt(0)
	, res_idx(0)
	, resolved(0)
{

}

ScopedCycleCounterStorage::NameStorage::~NameStorage()
{

}

void ScopedCycleCounterStorage::NameStorage::AddTick(Seconds sec)
{
	dur += sec.Value;
	cnt += 1;
}

void ScopedCycleCounterStorage::NameStorage::ResolveTick()
{
	float myt = dur / (float)cnt;
	dur = 0;
	cnt = 0;

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

	avg /= (float)res_cnt;
	resolved = avg;
}

Seconds ScopedCycleCounterStorage::NameStorage::ResolvedTick_get() const
{
	return resolved;
}

ScopedCycleCounterStorage::ScopedCycleCounterStorage(TRefPtr<String> storageName) : Super()
	, storageName(storageName)
{

}

ScopedCycleCounterStorage::~ScopedCycleCounterStorage()
{

}

TRefPtr<String> ScopedCycleCounterStorage::ToString() const
{
	struct Pair
	{
		Seconds Tick;
		TRefPtr<String> Name;

		Pair(Seconds sec, TRefPtr<String> name)
		{
			Tick = sec;
			Name = name;
		}
	};

	wostringstream woss;
	vector<Pair> container;

	woss << storageName->C_Str;
	
	for (auto& counter : tickStorage)
	{
		Seconds resolvedTick = counter.second.ResolvedTick;
		if (resolvedTick > 0s)
		{
			container.emplace_back(resolvedTick, counter.first);
		}
	}

	if (container.empty())
	{
		return L"";
	}

	sort(container.begin(), container.end(), [](const Pair& lh, const Pair& rh)
	{
		return lh.Tick > rh.Tick;
	});

	for (auto& p : container)
	{
		woss << endl << L'\t' << p.Name->C_Str << L": " << static_cast<Milliseconds>(p.Tick).ToString()->C_Str;
	}

	return woss.str().c_str();
}

void ScopedCycleCounterStorage::AddFrameDiag(ScopedCycleCounter& cycleCounter)
{
	auto it = tickStorage.find(cycleCounter.Name);
	if (it == tickStorage.end())
	{
		it = tickStorage.emplace(cycleCounter.Name, NameStorage()).first;
	}
	it->second.AddTick(cycleCounter.Secs);
}

void ScopedCycleCounterStorage::ResolveTicks()
{
	for (auto& item : tickStorage)
	{
		item.second.ResolveTick();
	}
}

TRefPtr<String> ScopedCycleCounterStorage::StorageName_get() const
{
	return storageName;
}

void ScopedCycleCounterStorage::AddCounter(ScopedCycleCounter& cycleCounter)
{
	auto it = globalStorages.find(cycleCounter.Namespace);
	if (it == globalStorages.end())
	{
		it = globalStorages.emplace(cycleCounter.Namespace, NewObject<ScopedCycleCounterStorage>(cycleCounter.Namespace)).first;
	}
	it->second->AddFrameDiag(cycleCounter);
}

TRefPtr<String> ScopedCycleCounterStorage::GetDiagStatics()
{
	wostringstream woss;
	bool bIgnoreEndl = true;

	for (auto& storage : globalStorages)
	{
		if (!bIgnoreEndl)
		{
			woss << endl;
		}
		woss << storage.second->ToString()->C_Str;
		bIgnoreEndl = false;
	}

	return woss.str().c_str();
}

void ScopedCycleCounterStorage::MoveFrame()
{
	for (auto& storage : globalStorages)
	{
		storage.second->ResolveTicks();
	}
}