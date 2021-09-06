// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Widgets/Panel/Panel.h"

SPanel::SPanel(const std::wstring& name) : Super(name)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

std::wstring SPanel::GenerateAutoNumberedName()
{
    static std::atomic<size_t> number;
    static std::wstring wc_name = ANSI_TO_WCHAR(typeid(SPanel).name());
    return std::format(L"{}_{}", wc_name, number++);
}