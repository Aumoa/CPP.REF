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
	const SWidget* _content = nullptr;

public:
	TSlotBase()
	{
	}

	TSlotClass&& operator [](const SWidget* content) &&
	{
		SetContent(content);
		return static_cast<TSlotClass&&>(std::move(*this));
	}

	void SetContent(const SWidget* content)
	{
		_content = content;
	}

	const SWidget* GetContent() const
	{
		return _content;
	}
};