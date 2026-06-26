// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Diagnostics/TestFixture.h"
#include <memory>
#include <vector>

namespace Ayla
{
	class UnitTestRunner
	{
	private:
		struct FixtureEntry
		{
			String Name;
			std::unique_ptr<TestFixture> Fixture;
		};

	private:
		std::vector<FixtureEntry> m_Fixtures;

	public:
		void AddFixture(String name, std::unique_ptr<TestFixture> fixture);

		Task<int32> RunAsync(std::stop_token cancellationToken = {});
	};
}
