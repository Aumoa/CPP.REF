// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SLATE_API SButton : public SCompoundWidget
{
	GENERATED_SLATE_BODY(SButton)

public:
	BEGIN_SLATE_ATTRIBUTE()
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR();

protected:
	virtual bool OnMouseButton(const NGeometry& AllottedGeometry, EGenericPlatformInputMouseButtonType ButtonType, const Vector2N& CursorPosition, bool bUp) override;
};