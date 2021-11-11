// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimationCurve.h"

template<template<class> class AnimCurve>
class SCustomAnimationCurve : public SAnimationCurve
{
	GENERATED_BODY(SCustomAnimationCurve<AnimCurve>)

public:
	using CurveFunction = std::function<void(float)>;

private:
	const float StartValue;
	const float EndValue;

	float Timing;
	AnimCurve<float>* SourceCurve = nullptr;
	CurveFunction Function;

public:
	SCustomAnimationCurve(float StartValue, float EndValue, float CurveStart, float CurveEnd, CurveFunction Function)
		: Super(L"CustomAnimationCurve", CurveStart, CurveEnd)
		, StartValue(StartValue)
		, EndValue(EndValue)

		, Timing(0)
		, Function(Function)
	{
		check((bool)Function);
		SourceCurve = NewObject<AnimCurve<float>>(Timing, StartValue, EndValue, CurveStart, CurveEnd);
	}

	virtual bool Tick(float InDeltaTime)
	{
		const bool bFinish = SourceCurve->Tick(InDeltaTime);
		Function(Timing);
		return bFinish;
	}

	virtual void Reset() override
	{
		Super::Reset();
		SourceCurve->Reset();
	}

protected:
	virtual void NormalizedTick(CurveTiming CurveTimingStart, CurveTiming CurveTimingEnd) override
	{
	}

	virtual SAnimationCurve* MakeClone() override
	{
		return NewObject<SCustomAnimationCurve<AnimCurve>>(StartValue, EndValue, GetCurveStartTime(), GetCurveEndTime(), Function);
	}
};