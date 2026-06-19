// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "UnitTestRunner.h"
#include "Console.h"

namespace Ayla
{
	void UnitTestRunner::AddFixture(String name, std::unique_ptr<TestFixture> fixture)
	{
		m_Fixtures.emplace_back(FixtureEntry
		{
			.Name = std::move(name),
			.Fixture = std::move(fixture)
		});
	}

	Task<int32> UnitTestRunner::RunAsync(std::stop_token cancellationToken)
	{
		int32 totalCases = 0;
		int32 failedCases = 0;
		int32 totalAssertions = 0;
		int32 failedAssertions = 0;

		Console::WriteLine(TEXT("Ayla Unit Tests"));

		for (auto& fixture : m_Fixtures)
		{
			Console::WriteLine(TEXT("[ RUN      ] {}"), fixture.Name);
			std::vector<TestFixture::TestCaseResult> results = co_await fixture.Fixture->StartAsync(cancellationToken);

			for (const auto& result : results)
			{
				++totalCases;
				totalAssertions += result.Passes + static_cast<int32>(result.Failures.size());
				failedAssertions += static_cast<int32>(result.Failures.size());

				if (result.Failures.empty() && result.ExceptionMessage.IsEmpty())
				{
					Console::WriteLine(TEXT("[       OK ] {}.{} ({} assertion(s))"), fixture.Name, result.Name, result.Passes);
					continue;
				}

				++failedCases;
				Console::Error.WriteLine(TEXT("[  FAILED  ] {}.{}"), fixture.Name, result.Name);
				if (result.ExceptionMessage.IsEmpty() == false)
				{
					Console::Error.WriteLine(TEXT("  {}"), result.ExceptionMessage);
				}

				for (const auto& failure : result.Failures)
				{
					Console::Error.WriteLine(TEXT("  {}"), failure);
				}
			}
		}

		Console::WriteLine(TEXT("{} test case(s), {} assertion(s), {} failed case(s), {} failed assertion(s)."), totalCases, totalAssertions, failedCases, failedAssertions);
		co_return failedCases == 0 && failedAssertions == 0 ? 0 : 1;
	}
}
