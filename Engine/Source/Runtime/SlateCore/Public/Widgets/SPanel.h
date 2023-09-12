// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SLATECORE_API SPanel : public SWidget
{
	GENERATED_SLATE_BODY(SPanel)

public:
	SPanel();

	virtual void PrepassLayout() override;

public:
	virtual size_t NumChildrens() const = 0;
	virtual SWidget* GetChildrenAt(size_t InIndex) const = 0;

public:
	BEGIN_SLATE_ATTRIBUTE()
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR() {}
};