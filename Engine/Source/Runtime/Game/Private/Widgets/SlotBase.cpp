// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/SlotBase.h"
#include "Widgets/SWidget.h"

SlotBase::SlotBase() noexcept
{
}

SlotBase::SlotBase(const SlotBase& rhs)
	: _content(rhs._content)
{
}

SlotBase::SlotBase(SlotBase&& rhs) noexcept
	: _content(std::move(rhs._content))
{
}

SlotBase::~SlotBase() noexcept
{
}

std::shared_ptr<SWidget> SlotBase::GetContent() const
{
	return _content;
}

void SlotBase::SetContent(const std::shared_ptr<SWidget>& inContent)
{
	_content = inContent;
}