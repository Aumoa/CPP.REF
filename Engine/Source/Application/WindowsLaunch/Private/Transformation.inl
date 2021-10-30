// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Layout/Layout.h"

inline D2D1_MATRIX_3X2_F GetRenderTransform(const SlateRenderTransform& InTransform)
{
	Matrix3x2 M = InTransform.GetMatrix3x2();
	return reinterpret_cast<const D2D1_MATRIX_3X2_F&>(M);
}

inline SlateRenderTransform GetRenderTransform(const D2D1_MATRIX_3X2_F& InTransform)
{
	return SlateRenderTransform(reinterpret_cast<const Matrix3x2&>(InTransform));
}

inline D2D1_COLOR_F GetColor(const Color& InColor)
{
	return reinterpret_cast<const D2D1_COLOR_F&>(InColor);
}

inline Color GetColor(const D2D1_COLOR_F& InColor)
{
	return reinterpret_cast<const Color&>(InColor);
}

inline D2D1_RECT_F GetRect(const Rect& InRect)
{
	return reinterpret_cast<const D2D1_RECT_F&>(InRect);
}

inline Rect GetRect(const D2D1_RECT_F& InRect)
{
	return reinterpret_cast<const Rect&>(InRect);
}