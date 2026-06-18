// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "Threading/Tasks/Task.h"
#include "Func.h"
#include <vector>
#include <optional>
#include <functional>

namespace Ayla
{
	class CORE_API TestFixture
	{
	public:
		struct TestCase
		{
			String Name;
			std::vector<std::function<Task<>(std::stop_token)>> TestFuncs;
		};

		struct TestCaseResult
		{
			String Name;
			int32 Passes = 0;
			std::vector<String> Failures;
			String ExceptionMessage;
		};

		struct CORE_API Assert
		{
		private:
			struct AcquisitionScope
			{
				~AcquisitionScope() noexcept
				{
					s_Current = nullptr;
				}
			};

		private:
			static TestCaseResult* s_Current;

		public:
			static AcquisitionScope AcquireCase(TestCaseResult& caseResult)
			{
				s_Current = &caseResult;
				return AcquisitionScope();
			}

			static void Equal(const auto& expected, const auto& actual)
			{
				check(s_Current);
				if (!(expected == actual))
				{
					s_Current->Failures.emplace_back(String::Format(TEXT("Assert::Equal failed: Expected '{}', Got '{}'"), expected, actual));
				}
				else
				{
					s_Current->Passes++;
				}
			}

			static void True(bool condition)
			{
				check(s_Current);
				if (!condition)
				{
					s_Current->Failures.emplace_back(TEXT("Assert::True failed: Condition is false"));
				}
				else
				{
					s_Current->Passes++;
				}
			}

			static void False(bool condition)
			{
				check(s_Current);
				if (condition)
				{
					s_Current->Failures.emplace_back(TEXT("Assert::False failed: Condition is true"));
				}
				else
				{
					s_Current->Passes++;
				}
			}

			static void Fail(String message)
			{
				check(s_Current);
				s_Current->Failures.emplace_back(std::move(message));
			}

			static void NotNull(const auto& ptr)
			{
				check(s_Current);
				if (ptr == nullptr)
				{
					s_Current->Failures.emplace_back(TEXT("Assert::NotNull failed: Pointer is null"));
				}
				else
				{
					s_Current->Passes++;
				}
			}
		};

	protected:
		TestFixture();

	public:
		virtual ~TestFixture() noexcept;

		Task<std::vector<TestCaseResult>> StartAsync(std::stop_token cancellationToken = {});

	protected:
		virtual std::vector<TestCase> SetUp();
		virtual void TearDown();
	};
}
