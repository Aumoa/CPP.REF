// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SWidget;

class GAME_API SWidgetSlot : implements SObject
{
	GENERATED_BODY(SWidgetSlot)

private:
	SPROPERTY(Content)
	SWidget* Content = nullptr;

public:
	SWidgetSlot();

	void SetContent(SWidget* InContent);
	SWidget* GetContent();
};

template<class TSlotClass>
class TSlotBase : public SWidgetSlot
{
public:
	using Super = SWidgetSlot;
	using This = TSlotBase<TSlotClass>;

public:
	TSlotBase() : Super()
	{
	}

	TSlotClass&& operator [](SWidget* InContent) &&
	{
		SetContent(InContent);
		return static_cast<TSlotClass&&>(std::move(*this));
	}

	TSlotClass& operator [](SWidget* InContent) &
	{
		SetContent(InContent);
		return static_cast<TSlotClass&>(*this);
	}
};