// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Diagnostics/TestFixture.h"
#include "Console.h"

namespace Ayla
{
	TestFixture::TestCaseResult* TestFixture::Assert::s_Current;

	TestFixture::TestFixture()
	{
	}


	TestFixture::~TestFixture() noexcept
	{
	}

	Task<std::vector<TestFixture::TestCaseResult>> TestFixture::StartAsync(std::stop_token cancellationToken)
	{
		bool hasSynchronizationContext = SynchronizationContext::GetCurrent() != nullptr;
		auto testCases = SetUp();
		std::vector<TestCaseResult> testResults;
		for (const auto& testCase : testCases)
		{
			auto& caseResult = testResults.emplace_back();
			caseResult.Name = testCase.Name;
			auto scope = Assert::AcquireCase(caseResult);
			for (const auto& testFunc : testCase.TestFuncs)
			{
				try
				{
					Task<> testTask = testFunc(cancellationToken);
					if (hasSynchronizationContext)
					{
						co_await testTask.ConfigureAwait(true);
					}
					else
					{
						testTask.Wait();
					}
				}
				catch (const Exception& ex)
				{
					caseResult.ExceptionMessage = String::Format(TEXT("Test case '{}' threw an exception: {}"), testCase.Name, ex.ToString());
					Console::WriteLine(caseResult.ExceptionMessage);
				}
				catch (const std::exception& ex)
				{
					caseResult.ExceptionMessage = String::Format(TEXT("Test case '{}' threw an exception: {}"), testCase.Name, String::FromLiteral(ex.what()));
					Console::WriteLine(caseResult.ExceptionMessage);
					continue;
				}
				catch (...)
				{
					caseResult.ExceptionMessage = String::Format(TEXT("Test case '{}' threw an unknown exception"), testCase.Name);
					Console::WriteLine(caseResult.ExceptionMessage);
					continue;
				}
			}
		}

		TearDown();
		co_return std::move(testResults);
	}

	std::vector<TestFixture::TestCase> TestFixture::SetUp()
	{
		return {};
	}

	void TestFixture::TearDown()
	{
	}
}