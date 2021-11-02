// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

enum class EFlowDirection
{
	LeftToRight,
	RightToLeft
};

enum class EOrientation
{
	Horizontal,
	Vertical
};

enum class ESlateVisibility
{
	Visible,
	Collapsed,
	Hidden,
	HitTestInvisible,
	SelfHitTestInvisible
};

enum class EWidgetClipping
{
	Inherit,
	ClipToBounds,
	ClipToBoundsWithoutIntersection,
};

enum class EHorizontalAlignment
{
	Fill,
	Left,
	Center,
	Right,
};

enum class EVerticalAlignment
{
	Fill,
	Top,
	Center,
	Bottom,
};

enum class ESizeRule
{
	Auto,
	Stretch
};

class SlateVisibilityExtensions abstract
{
	static constexpr int32 VISPRIVATE_Visible = 0x1 << 0;
	static constexpr int32 VISPRIVATE_Collapsed = 0x1 << 1;
	static constexpr int32 VISPRIVATE_Hidden = 0x1 << 2;
	static constexpr int32 VISPRIVATE_SelfHitTestVisible = 0x1 << 3;
	static constexpr int32 VISPRIVATE_ChildrenHitTestVisible = 0x1 << 4;

	static constexpr int32 VIS_Visible = VISPRIVATE_Visible | VISPRIVATE_SelfHitTestVisible | VISPRIVATE_ChildrenHitTestVisible;
	static constexpr int32 VIS_Collapsed = VISPRIVATE_Collapsed;
	static constexpr int32 VIS_Hidden = VISPRIVATE_Hidden;
	static constexpr int32 VIS_HitTestInvisible = VISPRIVATE_Visible;
	static constexpr int32 VIS_SelfHitTestInvisible = VISPRIVATE_Visible | VISPRIVATE_ChildrenHitTestVisible;

	static constexpr int32 VIS_All = VISPRIVATE_Visible | VISPRIVATE_Hidden | VISPRIVATE_Collapsed | VISPRIVATE_SelfHitTestVisible | VISPRIVATE_ChildrenHitTestVisible;

public:
	static bool DoesVisibilityPassFilter(ESlateVisibility Visibility, ESlateVisibility Filter)
	{
		return 0 != (GetValue(Visibility) & GetValue(Filter));
	}

	static bool AreChildrenHitTestVisible(ESlateVisibility Visibility)
	{
		return 0 != (GetValue(Visibility) & VISPRIVATE_ChildrenHitTestVisible);
	}

	static bool IsHitTestVisible(ESlateVisibility Visibility)
	{
		return 0 != (GetValue(Visibility) & VISPRIVATE_SelfHitTestVisible);
	}

	static bool IsVisible(ESlateVisibility Visibility)
	{
		return 0 != (GetValue(Visibility) & VIS_Visible);
	}

	static std::wstring ToString(ESlateVisibility Visibility, std::wstring_view InFormatArgs = L"")
	{
		if (InFormatArgs == L"base")
		{
			return std::format(L"ESlateVisibility:{}", GetName(Visibility));
		}
		else
		{
			return GetName(Visibility);
		}
	}

private:
	static int32 GetValue(ESlateVisibility Visibility)
	{
		switch (Visibility)
		{
		case ESlateVisibility::Visible: return VIS_Visible;
		case ESlateVisibility::Collapsed: return VIS_Collapsed;
		case ESlateVisibility::Hidden: return VIS_Hidden;
		case ESlateVisibility::HitTestInvisible: return VIS_HitTestInvisible;
		case ESlateVisibility::SelfHitTestInvisible: return VIS_SelfHitTestInvisible;
		default:
			check(false);
			return 0;
		};
	}

	static std::wstring GetName(ESlateVisibility Visibility)
	{
		switch (Visibility)
		{
		case ESlateVisibility::Visible: return L"Visible";
		case ESlateVisibility::Collapsed: return L"Collapsed";
		case ESlateVisibility::Hidden: return L"Hidden";
		case ESlateVisibility::HitTestInvisible: return L"HitTestInvisible";
		case ESlateVisibility::SelfHitTestInvisible: return L"SelfHitTestInvisible";
		default: return std::format(L"({})", (int32)Visibility);
		};
	}
};