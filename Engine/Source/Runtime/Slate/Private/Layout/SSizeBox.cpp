// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Layout/SSizeBox.h"

SSizeBox::SSizeBox()
{
}

SSizeBox::~SSizeBox() noexcept
{
}

DEFINE_SLATE_CONSTRUCTOR(SSizeBox, Args)
{
	OverrideWidth = Args._OverrideWidth;
	OverrideHeight = Args._OverrideHeight;
}

Vector2 SSizeBox::ComputeDesiredSize() const
{
	Vector2 Size;
	if (GetContent())
	{
		Size = GetContent()->GetDesiredSize();
	}
	return Vector2(OverrideWidth.value_or(Size.X), OverrideHeight.value_or(Size.Y));
}