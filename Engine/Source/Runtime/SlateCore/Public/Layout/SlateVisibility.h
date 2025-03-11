// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace ESlateVisibility
{
	enum Enum : uint8
	{
		Visible,
		Collapsed,
		Hidden,
		HitTestInvisible,
		SelfHitTestInvisible,
		All
	};

	inline constexpr uint8 VISPRIVATE_Visible = 0x1 << 0;
	inline constexpr uint8 VISPRIVATE_Collapsed = 0x1 << 1;
	inline constexpr uint8 VISPRIVATE_Hidden = 0x1 << 2;
	inline constexpr uint8 VISPRIVATE_SelfHitTestVisible = 0x1 << 3;
	inline constexpr uint8 VISPRIVATE_ChildrenHitTestVisible = 0x1 << 4;

	inline constexpr uint8 VIS_Visible = VISPRIVATE_Visible | VISPRIVATE_SelfHitTestVisible | VISPRIVATE_ChildrenHitTestVisible;
	inline constexpr uint8 VIS_Collapsed = VISPRIVATE_Collapsed;
	inline constexpr uint8 VIS_Hidden = VISPRIVATE_Hidden;
	inline constexpr uint8 VIS_HitTestInvisible = VISPRIVATE_Visible;
	inline constexpr uint8 VIS_SelfHitTestInvisible = VISPRIVATE_Visible | VISPRIVATE_ChildrenHitTestVisible;

	inline constexpr uint8 VIS_All = VISPRIVATE_Visible | VISPRIVATE_Hidden | VISPRIVATE_Collapsed | VISPRIVATE_SelfHitTestVisible | VISPRIVATE_ChildrenHitTestVisible;

	inline uint8 GetValue(ESlateVisibility::Enum InVisibility)
	{
		switch (InVisibility)
		{
		case ESlateVisibility::Visible: return VIS_Visible;
		case ESlateVisibility::Collapsed: return VIS_Collapsed;
		case ESlateVisibility::Hidden: return VIS_Hidden;
		case ESlateVisibility::HitTestInvisible: return VIS_HitTestInvisible;
		case ESlateVisibility::SelfHitTestInvisible: return VIS_SelfHitTestInvisible;
		case ESlateVisibility::All: return VIS_All;
		default:
			checkf(false, TEXT("Invalid argument: Visibility({})"), (uint8)InVisibility);
			return VIS_Hidden;
		};
	}

	inline String ToString(ESlateVisibility::Enum InVisibility)
	{
		switch (InVisibility)
		{
		case ESlateVisibility::Visible: return TEXT("Visible");
		case ESlateVisibility::Collapsed: return TEXT("Collapsed");
		case ESlateVisibility::Hidden: return TEXT("Hidden");
		case ESlateVisibility::HitTestInvisible: return TEXT("HitTestInvisible");
		case ESlateVisibility::SelfHitTestInvisible: return TEXT("SelfHitTestInvisible");
		case ESlateVisibility::All: return TEXT("All");
		default: return String::Format(TEXT("(ESlateVisibility){}"), (uint8)InVisibility);
		};
	}

	inline bool DoesVisibilityPassFilter(ESlateVisibility::Enum InVisibility, ESlateVisibility::Enum InFilter)
	{
		return 0 != (GetValue(InVisibility) & GetValue(InFilter));
	}

	inline bool AreChildrenHitTestVisible(ESlateVisibility::Enum InVisibility)
	{
		return 0 != (GetValue(InVisibility) & VISPRIVATE_ChildrenHitTestVisible);
	}

	inline bool IsHitTestVisible(ESlateVisibility::Enum InVisibility)
	{
		return 0 != (GetValue(InVisibility) & VISPRIVATE_SelfHitTestVisible);
	}

	inline bool IsVisible(ESlateVisibility::Enum InVisibility)
	{
		return 0 != (GetValue(InVisibility) & VIS_Visible);
	}
}