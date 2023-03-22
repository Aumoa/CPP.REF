// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <functional>

class CORE_API Finally
{
	std::function<void()> Action;

public:
	Finally(std::function<void()> Action);
	~Finally() noexcept;

	void Execute();
};