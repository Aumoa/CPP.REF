// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Layout/ArrangedChildrens.h"

template<class UPanel>
struct NPanelSlotBase
{
	using This = UPanel;
	std::shared_ptr<SWidget> Content;

	UPanel&& operator [](std::shared_ptr<SWidget> InContent) &&
	{
		Content = std::move(InContent);
		return static_cast<UPanel&&>(std::move(*this));
	}

	UPanel& operator [](std::shared_ptr<SWidget> InContent) &
	{
		Content = std::move(InContent);
		return static_cast<UPanel&>(*this);
	}
};

class SLATECORE_API SPanel : public SCompoundWidget
{
public:
	SPanel();

public:
	virtual size_t NumChildrens() const = 0;
	virtual SWidget* GetChildrenAt(size_t InIndex) const = 0;

public:
	BEGIN_SLATE_ATTRIBUTE(SPanel)
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR() {}
};