// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Tests/ManagedInteropTest.h"
#include "ScriptingBackend/ScriptingBackend.h"

namespace Ayla
{
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
						auto function = reinterpret_cast<signature_t>(ScriptingBackend::Get().GetFunctionPointer(
							"AylaUnitTests.Script",
							"Ayla.Tests.ManagedInteropSmoke",
							"Add"));

						Assert::Equal(7, function(3, 4));

						return Task<>::CompletedTask();
					}
				}
			}
		};
	}
}
