// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Animation/SlateAnimationContext.h"
#include "AnimCurves/AnimationCurve.h"
#include <ranges>

GENERATE_BODY(SSlateAnimationContext);

SSlateAnimationContext::SSlateAnimationContext(std::wstring_view AnimationName) : Super()
	, Name(AnimationName)
{
}

std::wstring SSlateAnimationContext::ToString()
{
	return GetName();
}

std::wstring SSlateAnimationContext::GetName()
{
	return Name;
}

bool SSlateAnimationContext::AddCurve(SAnimationCurve* Curve)
{
	AnimCurves.emplace_back(Curve);
	return true;
}

bool SSlateAnimationContext::RemoveCurve(SAnimationCurve* Curve)
{
	if (auto It = std::find(AnimCurves.begin(), AnimCurves.end(), Curve); It != AnimCurves.end())
	{
		AnimCurves.erase(It);
		return true;
	}

	return false;
}

void SSlateAnimationContext::InitCurves()
{
	StopAnimation();

	QueuedAnimCurves = AnimCurves;
	PlayCurves.reserve(QueuedAnimCurves.size());
	std::sort(QueuedAnimCurves.begin(), QueuedAnimCurves.end(), std::greater_equal<>());

	for (auto& Curve : AnimCurves)
	{
		Curve->Reset();
	}

	ElapsedTime = 0;
	bCurveInit = true;
}

void SSlateAnimationContext::Tick(float InDeltaTime)
{
	if (bCurveInit)
	{
		const float StartTiming = ElapsedTime;
		const float EndTiming = ElapsedTime += InDeltaTime;

		// Queueing anim curves.
		size_t NumRemoves = 0;
		for (auto& Curve : std::views::reverse(QueuedAnimCurves))
		{
			if (Curve->IsTimingInRange(StartTiming, EndTiming))
			{
				NumRemoves += 1;
				PlayCurves.emplace_back(Curve);
			}
			else
			{
				break;
			}
		}

		if (NumRemoves)
		{
			auto End = QueuedAnimCurves.end();
			auto Begin = End - NumRemoves;
			QueuedAnimCurves.erase(Begin, End);
		}

		// Play curves.
		for (size_t i = 0; i < PlayCurves.size(); ++i)
		{
			SAnimationCurve* Curve = PlayCurves[i];
			Curve->Tick(InDeltaTime);

			if (!Curve->IsTimingInRange(EndTiming))
			{
				PlayCurves.erase(PlayCurves.begin() + i);
				i -= 1;
				continue;
			}
		}

		if (QueuedAnimCurves.empty() && PlayCurves.empty())
		{
			StopAnimation();
		}
	}
}

void SSlateAnimationContext::StopAnimation()
{
	if (bCurveInit)
	{
		QueuedAnimCurves.clear();
		PlayCurves.clear();
		ElapsedTime = 0;
		bCurveInit = false;
	}
}

bool SSlateAnimationContext::IsPlaying()
{
	return bCurveInit;
}

SSlateAnimationContext* SSlateAnimationContext::Clone()
{
	SSlateAnimationContext* NewContext = gcnew SSlateAnimationContext(Name);
	NewContext->AnimCurves.reserve(AnimCurves.size());
	NewContext->ElapsedTime = 0;
	NewContext->bCurveInit = false;

	for (auto& Curve : AnimCurves)
	{
		SAnimationCurve* CloneCurve = Curve->Clone();
		NewContext->AnimCurves.emplace_back(CloneCurve);
	}

	return NewContext;
}