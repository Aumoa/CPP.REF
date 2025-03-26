// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

namespace Ayla
{
	struct StaticClass
	{
		StaticClass() = delete;
		StaticClass(const StaticClass&) = delete;
		StaticClass(StaticClass&&) = delete;
	};
}