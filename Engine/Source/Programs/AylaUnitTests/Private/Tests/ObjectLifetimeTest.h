// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Diagnostics/TestFixture.h"

namespace Ayla
{
	class ObjectLifetimeTest : public TestFixture
	{
	protected:
		virtual std::vector<TestCase> SetUp() override;
	};
}
