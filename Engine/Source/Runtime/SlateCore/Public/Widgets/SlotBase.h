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
	SWidget* _content = nullptr;

public:
	TSlotBase()
	{
	}

	TSlotClass&& operator [](SWidget* content) &&
	{
		SetContent(content);
		return static_cast<TSlotClass&&>(std::move(*this));
	}

	void SetContent(SWidget* content)
	{
		_content = content;
	}

	SWidget* GetContent() const
	{
		return _content;
	}
};