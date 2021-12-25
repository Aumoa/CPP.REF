// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#define DECLARE_STAT_GROUP(Name, Group) static CycleCounterNamespace Group(L ## Name, L ## #Group)
#define DECLARE_CYCLE_STAT(Name, Stat, Group) static CycleCounterUnit Stat(&Group, L ## Name)
#define SCOPE_CYCLE_COUNTER(Stat) auto Stat ## _Instance = Stat.GetScopedTimer()