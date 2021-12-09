// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "AnimCurves/AnimationCurve.h"
#include "AnimCurves/LinearAnimationCurve.h"

GENERATE_BODY(SAnimationCurve);

SAnimationCurve::SAnimationCurve(std::wstring_view CurveName, float InCurveStart, float InCurveEnd) : Super()
	, CurveName(CurveName)
{
	if (ensure(InCurveStart < InCurveEnd))
	{
		CurveStart = InCurveStart;
		CurveEnd = InCurveEnd;
	}
	else
	{
		CurveStart = InCurveEnd;
		CurveEnd = InCurveStart;
	}

	CurveDuration = CurveEnd - CurveStart;
	InvCurveDuration = 1.0f / CurveDuration;
	ElapsedTime = 0;
}

std::wstring SAnimationCurve::ToString()
{
	return std::format(L"{} @ CurveName: {}", Super::ToString(), CurveName);
}

bool SAnimationCurve::Tick(float InDeltaTime)
{
	const float TimingBegin = ElapsedTime;
	const float TimingEnd = AdvanceTiming(InDeltaTime);

	NormalizedTick(GetCurveTiming(TimingBegin), GetCurveTiming(TimingEnd));

	return IsEndOfCurve();
}

void SAnimationCurve::Reset()
{
	ElapsedTime = 0;
}

bool SAnimationCurve::IsEndOfCurve()
{
	return ElapsedTime >= CurveDuration;
}

bool SAnimationCurve::IsTimingInRange(float InTiming)
{
	return CurveStart <= InTiming && InTiming <= CurveEnd;
}

bool SAnimationCurve::IsTimingInRange(float InStartTiming, float InEndTiming)
{
	return CurveStart <= InEndTiming && InStartTiming <= CurveEnd;
}

std::wstring SAnimationCurve::GetCurveName()
{
	return CurveName;
}

float SAnimationCurve::GetCurveDuration()
{
	return CurveDuration;
}

float SAnimationCurve::GetCurveStartTime()
{
	return CurveStart;
}

float SAnimationCurve::GetCurveEndTime()
{
	return CurveEnd;
}

SAnimationCurve* SAnimationCurve::Clone()
{
	SAnimationCurve* CloneObject = MakeClone();
	checkf(GetType()->IsA(CloneObject->GetType()), L"Type mismatch detected! Please override MakeClone() function to create clone object.");
	return CloneObject;
}

float SAnimationCurve::AdvanceTiming(float InDeltaTime)
{
	ElapsedTime += InDeltaTime;
	if (ElapsedTime >= CurveDuration)
	{
		ElapsedTime = CurveDuration;
	}
	return ElapsedTime;
}

auto SAnimationCurve::GetCurveTiming(float Timing) -> CurveTiming
{
	return CurveTiming
	{
		.AbsoluteTiming = Timing,
		.Timing = Timing * InvCurveDuration
	};
}