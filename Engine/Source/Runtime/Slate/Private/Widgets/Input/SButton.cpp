// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Widgets/Input/SButton.h"

DEFINE_SLATE_CONSTRUCTOR(SButton, Args)
{
}

bool SButton::OnMouseButton(const NGeometry& AllottedGeometry, EGenericPlatformInputMouseButtonType ButtonType, const Vector2N& CursorPosition, bool bUp)
{
	return false;
}