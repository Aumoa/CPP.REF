// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class GAME_API ScopedCycleCounter : virtual public Object
{
public:
	using Super = Object;
	using This = ScopedCycleCounter;
	using MyTimer = std::chrono::steady_clock;

private:
	TRefPtr<String> nspace;
	TRefPtr<String> name;

	MyTimer::time_point start;
	MyTimer::duration mydur;

#if defined(_DEBUG)
	size_t debugThreadId;
#endif

public:
	ScopedCycleCounter(TRefPtr<String> inNamespace, TRefPtr<String> inName);
	~ScopedCycleCounter() override;

	vs_property_get(TRefPtr<String>, Namespace);
	TRefPtr<String> Namespace_get() const;
	vs_property_get(TRefPtr<String>, Name);
	TRefPtr<String> Name_get() const;
	vs_property_get(Seconds, Secs);
	Seconds Secs_get() const;

private:
	void RegisterFrameDiagnostics();
};

#define QUICK_SCOPED_CYCLE_COUNTER(InNamespace, InScopeName) ScopedCycleCounter InNamespace ## _ ## InScopeName(L ## #InNamespace, L ## #InScopeName)