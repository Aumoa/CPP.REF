// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Widgets/LeafWidget.h"

SLeafWidget::SLeafWidget(const std::wstring& name) : Super(name)
{
}

SLeafWidget::~SLeafWidget()
{
}

void SLeafWidget::OnArrangeChildren(ArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) const
{
}

std::wstring SLeafWidget::GenerateAutoNumberedName()
{
    static std::atomic<size_t> number;
    static std::wstring wc_name = ANSI_TO_WCHAR(typeid(SLeafWidget).name());
    return std::format(L"{}_{}", wc_name, number++);
}