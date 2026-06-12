// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Tests/ManagedInteropTest.h"
#include "ScriptingBackend/ScriptingBackend.h"

namespace Ayla
{
	namespace
	{
		int32 MultiplyForManagedCallback(int32 left, int32 right)
		{
			return left * right;
		}

		template<class T>
		T GetManagedInteropFunction(std::string_view methodName)
		{
			return reinterpret_cast<T>(ScriptingBackend::Get().GetFunctionPointer(
				"AylaUnitTests.Script",
				"Ayla.Tests.ManagedInteropSmoke",
				methodName));
		}
	}

	std::vector<TestFixture::TestCase> ManagedInteropTest::SetUp()
	{
		return
		{
			TestCase
			{
				.Name = TEXT("Managed delegate invocation"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						using signature_t = int32(*)(int32, int32);
						auto function = GetManagedInteropFunction<signature_t>("Add");

						Assert::Equal(7, function(3, 4));

						return Task<>::CompletedTask();
					}
				}
			},
			TestCase
			{
				.Name = TEXT("Managed callback invocation"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						using signature_t = int32(*)(ssize_t, int32, int32);
						auto function = GetManagedInteropFunction<signature_t>("InvokeNativeCallback");

						Assert::Equal(42, function(reinterpret_cast<ssize_t>(&MultiplyForManagedCallback), 6, 7));
						Assert::Equal(-12, function(reinterpret_cast<ssize_t>(&MultiplyForManagedCallback), -3, 4));

						return Task<>::CompletedTask();
					}
				}
			},
			TestCase
			{
				.Name = TEXT("Managed static state persists between calls"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						using signature_t = int32(*)(int32);
						auto resetCounter = GetManagedInteropFunction<signature_t>("ResetCounter");
						auto incrementCounter = GetManagedInteropFunction<signature_t>("IncrementCounter");

						Assert::Equal(10, resetCounter(10));
						Assert::Equal(15, incrementCounter(5));
						Assert::Equal(12, incrementCounter(-3));

						return Task<>::CompletedTask();
					}
				}
			}
		};
	}
}
