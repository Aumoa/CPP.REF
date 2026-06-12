// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Tests/ObjectLifetimeTest.h"
#include "Tests/LifetimeTestObject.h"
#include "Marshal/ObjectReferenceWrapper.h"
#include "ScriptingBackend/ScriptingBackend.h"

namespace Ayla
{
	namespace
	{
		template<class T>
		T GetManagedLifetimeFunction(std::string_view methodName)
		{
			return reinterpret_cast<T>(ScriptingBackend::Get().GetFunctionPointer(
				"AylaUnitTests.Script",
				"Ayla.Tests.ManagedObjectLifetimeSmoke",
				methodName));
		}
	}

	std::vector<TestFixture::TestCase> ObjectLifetimeTest::SetUp()
	{
		return
		{
			TestCase
			{
				.Name = TEXT("Native reference release destroys object"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						LifetimeTestObject::ResetCounters();
						{
							auto object = Object::New<LifetimeTestObject>();
							Assert::Equal(1, LifetimeTestObject::GetConstructedCount());
							Assert::Equal(0, LifetimeTestObject::GetDestroyedCount());
						}

						Assert::Equal(1, LifetimeTestObject::GetDestroyedCount());
						return Task<>::CompletedTask();
					}
				}
			},
			TestCase
			{
				.Name = TEXT("Managed wrapper keeps native object until dispose"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						using hold_t = ssize_t(*)(ObjectReferenceWrapper);
						using dispose_t = void(*)();

						auto hold = GetManagedLifetimeFunction<hold_t>("Hold");
						auto disposeHeld = GetManagedLifetimeFunction<dispose_t>("DisposeHeld");

						LifetimeTestObject::ResetCounters();
						auto object = Object::New<LifetimeTestObject>();
						auto nativePointer = reinterpret_cast<ssize_t>(object.Get());
						auto wrapper = ObjectReferenceWrapper::FromObject(object);

						Assert::Equal(nativePointer, hold(wrapper));
						Assert::Equal(0, LifetimeTestObject::GetDestroyedCount());

						object.Release();
						Assert::Equal(0, LifetimeTestObject::GetDestroyedCount());

						disposeHeld();
						Assert::Equal(1, LifetimeTestObject::GetDestroyedCount());

						return Task<>::CompletedTask();
					}
				}
			},
			TestCase
			{
				.Name = TEXT("Managed finalizer releases native object"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						using create_unheld_t = ssize_t(*)(ObjectReferenceWrapper);
						using collect_t = void(*)();

						auto createUnheld = GetManagedLifetimeFunction<create_unheld_t>("CreateUnheld");
						auto forceFullCollection = GetManagedLifetimeFunction<collect_t>("ForceFullCollection");

						LifetimeTestObject::ResetCounters();
						auto object = Object::New<LifetimeTestObject>();
						auto nativePointer = reinterpret_cast<ssize_t>(object.Get());
						auto wrapper = ObjectReferenceWrapper::FromObject(object);

						Assert::Equal(nativePointer, createUnheld(wrapper));
						Assert::Equal(0, LifetimeTestObject::GetDestroyedCount());

						object.Release();
						Assert::Equal(0, LifetimeTestObject::GetDestroyedCount());

						forceFullCollection();
						Assert::Equal(1, LifetimeTestObject::GetDestroyedCount());

						return Task<>::CompletedTask();
					}
				}
			}
		};
	}
}
