// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SWidget;

template<class TSlotClass>
class TSlotBase
{
public:
	using This = TSlotClass;

private:
	SWidget* Content = nullptr;

public:
	TSlotBase()
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

	void SetContent(SWidget* InContent)
	{
		Content = InContent;
	}

	SWidget* GetContent() const
	{
		return Content;
	}
};