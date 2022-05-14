// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#define DECLARE_STAT_GROUP(Name, Group) static ::libty::CycleCounterNamespace Group(TEXT(Name), TEXT(#Group))
#define DECLARE_CYCLE_STAT(Name, Stat, Group) static ::libty::CycleCounterUnit Stat(&Group, TEXT(Name))
#define SCOPE_CYCLE_COUNTER(Stat) auto Stat ## _Instance = Stat.GetScopedTimer()