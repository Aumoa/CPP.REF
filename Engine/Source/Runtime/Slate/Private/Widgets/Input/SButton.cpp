// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Widgets/Input/SButton.h"

DEFINE_SLATE_CONSTRUCTOR(SButton, Args)
{
	PLATFORM_UNREFERENCED_PARAMETER(Args);
}

bool SButton::OnMouseButton(const NGeometry& AllottedGeometry, EGenericPlatformInputMouseButtonType ButtonType, const Vector2N& CursorPosition, bool bUp)
{
	PLATFORM_UNREFERENCED_PARAMETER(AllottedGeometry);
	PLATFORM_UNREFERENCED_PARAMETER(ButtonType);
	PLATFORM_UNREFERENCED_PARAMETER(CursorPosition);
	PLATFORM_UNREFERENCED_PARAMETER(bUp);
	return false;
}