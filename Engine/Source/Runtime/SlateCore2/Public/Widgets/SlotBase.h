// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <memory>

class SWidget;

class SlotBase
{
private:
	std::shared_ptr<SWidget> _content;

public:
	SlotBase() noexcept;
	SlotBase(const SlotBase& rhs);
	SlotBase(SlotBase&& rhs) noexcept;
	virtual ~SlotBase() noexcept;

	std::shared_ptr<SWidget> GetContent() const;
	void SetContent(const std::shared_ptr<SWidget>& inContent);

	template<class TSelf>
	auto operator [](this TSelf&& self, const std::shared_ptr<SWidget>& inContent)
	{
		SetContent(inContent);
		return std::forward<TSelf>(self);
	}
};