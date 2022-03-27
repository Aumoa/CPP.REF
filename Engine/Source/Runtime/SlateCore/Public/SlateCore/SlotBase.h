// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"

class SWidget;

namespace libty::SlateCore::Details
{
	class SLATECORE_API SSlotBase : implements SObject
	{
		GENERATED_BODY(SSlotBase)

	private:
		SPROPERTY(Content)
		SWidget* Content = nullptr;

	public:
		SSlotBase();

		SWidget* GetContent();
		void SetContent(SWidget* Content);
	};
}

template<class TSlotClass>
class TSlotBase : public libty::SlateCore::Details::SSlotBase
{
public:
	using Super = libty::SlateCore::Details::SSlotBase;
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