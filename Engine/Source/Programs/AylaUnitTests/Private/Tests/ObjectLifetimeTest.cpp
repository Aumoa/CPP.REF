// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Tests/ObjectLifetimeTest.h"
#include "Tests/LifetimeTestObject.h"
#include "Activator.h"
#include "ManagedException.h"
#include "Marshal/ObjectReferenceWrapper.h"
#include "Reflection/TypeCollector.h"
#include "ScriptingBackend/ScriptingBackend.h"
#include "Type.h"

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
				.Name = TEXT("Native type metadata matches object instances"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						Type* objectType = TypeCollector::FindType(typeid(Object));
						Type* lifetimeType = TypeCollector::FindType(typeid(LifetimeTestObject));

						Assert::NotNull(objectType);
						Assert::NotNull(lifetimeType);

						Assert::Equal(TEXT("Ayla"), objectType->GetNamespace());
						Assert::Equal(TEXT("Object"), objectType->GetName());
						Assert::Equal(TEXT("Ayla.Object"), objectType->GetFullName());
						Assert::True(*objectType->GetTypeInfo() == typeid(Object));

						Assert::Equal(TEXT("Ayla"), lifetimeType->GetNamespace());
						Assert::Equal(TEXT("LifetimeTestObject"), lifetimeType->GetName());
						Assert::Equal(TEXT("Ayla.LifetimeTestObject"), lifetimeType->GetFullName());
						Assert::True(*lifetimeType->GetTypeInfo() == typeid(LifetimeTestObject));

						auto object = Object::New<Object>();
						auto lifetimeObject = Object::New<LifetimeTestObject>();

						Assert::True(object->GetType() == objectType);
						Assert::True(lifetimeObject->GetType() == lifetimeType);

						return Task<>::CompletedTask();
					}
				}
			},
			TestCase
			{
				.Name = TEXT("Managed type wrappers point back to native types"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						Type* objectType = TypeCollector::FindType(typeid(Object));
						Type* lifetimeType = TypeCollector::FindType(typeid(LifetimeTestObject));

						Assert::NotNull(objectType);
						Assert::NotNull(lifetimeType);

						const ManagedTypeWrapper objectManagedType = objectType->GetManagedType();
						Assert::True(objectManagedType.NativeType == objectType);
						Assert::NotNull(objectManagedType.ScriptTypeGetter);

						const ManagedTypeWrapper lifetimeManagedType = lifetimeType->GetManagedType();
						Assert::True(lifetimeManagedType.NativeType == lifetimeType);
						Assert::NotNull(lifetimeManagedType.ScriptTypeGetter);

						auto lifetimeObject = Object::New<LifetimeTestObject>();
						const ManagedTypeWrapper instanceManagedType = lifetimeObject->GetType()->GetManagedType();
						Assert::True(instanceManagedType.NativeType == lifetimeObject->GetType());
						Assert::NotNull(instanceManagedType.ScriptTypeGetter);

						return Task<>::CompletedTask();
					}
				}
			},
			TestCase
			{
				.Name = TEXT("Managed activation failure crosses CoreCLRFunctions boundary"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						Type* lifetimeType = TypeCollector::FindType(typeid(LifetimeTestObject));
						Assert::NotNull(lifetimeType);

						bool caughtManagedException = false;
						try
						{
							SharedPtr<Object> object = Activator::CreateInstance(lifetimeType->GetManagedType());
							PLATFORM_UNREFERENCED_PARAMETER(object);
						}
						catch (const ManagedException& ex)
						{
							caughtManagedException = true;
							Assert::True(ex.GetManagedTypeName().Contains(TEXT("MissingMethodException")));
							Assert::True(ex.GetManagedDetails().Contains(TEXT("ManagedLifetimeObject")));
							Assert::True(ex.GetManagedExceptionToken() != 0);
						}

						Assert::True(caughtManagedException);

						return Task<>::CompletedTask();
					}
				}
			},
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
			},
			TestCase
			{
				.Name = TEXT("Managed wrapper replaces stale weak handle"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						using replace_t = ssize_t(*)(ObjectReferenceWrapper);
						using dispose_t = void(*)();

						auto replaceStaleWeakHandle = GetManagedLifetimeFunction<replace_t>("ReplaceStaleWeakHandle");
						auto disposeHeld = GetManagedLifetimeFunction<dispose_t>("DisposeHeld");

						LifetimeTestObject::ResetCounters();
						auto object = Object::New<LifetimeTestObject>();
						auto nativePointer = reinterpret_cast<ssize_t>(object.Get());
						auto wrapper = ObjectReferenceWrapper::FromObject(object);

						Assert::Equal(nativePointer, replaceStaleWeakHandle(wrapper));
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
				.Name = TEXT("Managed wrapper replacement waits for pending finalizer"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						using create_pending_t = ssize_t(*)(ObjectReferenceWrapper);
						using hold_after_pending_t = ssize_t(*)(ObjectReferenceWrapper);
						using dispose_t = void(*)();

						auto createPendingFinalizer = GetManagedLifetimeFunction<create_pending_t>("CreatePendingFinalizer");
						auto holdAfterPendingFinalizer = GetManagedLifetimeFunction<hold_after_pending_t>("HoldAfterPendingFinalizer");
						auto disposeHeld = GetManagedLifetimeFunction<dispose_t>("DisposeHeld");

						LifetimeTestObject::ResetCounters();
						auto object = Object::New<LifetimeTestObject>();
						auto nativePointer = reinterpret_cast<ssize_t>(object.Get());

						auto firstWrapper = ObjectReferenceWrapper::FromObject(object);
						Assert::Equal(nativePointer, createPendingFinalizer(firstWrapper));
						Assert::Equal(0, LifetimeTestObject::GetDestroyedCount());

						auto secondWrapper = ObjectReferenceWrapper::FromObject(object);
						Assert::Equal(nativePointer, holdAfterPendingFinalizer(secondWrapper));
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
				.Name = TEXT("Native wrapper consumption clears intermediate handle"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						using create_wrapper_t = void(*)(ObjectReferenceWrapper, ObjectReferenceWrapper*);
						using dispose_t = void(*)();

						auto createWrapper = GetManagedLifetimeFunction<create_wrapper_t>("CreateHeldWrapperForNativeConsumption");
						auto disposeHeld = GetManagedLifetimeFunction<dispose_t>("DisposeHeld");

						LifetimeTestObject::ResetCounters();
						auto object = Object::New<LifetimeTestObject>();
						auto sourceWrapper = ObjectReferenceWrapper::FromObject(object);

						ObjectReferenceWrapper nativeWrapper{};
						createWrapper(sourceWrapper, &nativeWrapper);
						Assert::True(nativeWrapper.IntGCHandlePtr != 0);

						{
							auto firstNative = nativeWrapper.AsNative<LifetimeTestObject>();
							Assert::NotNull(firstNative.Get());
							Assert::Equal((ssize_t)0, nativeWrapper.IntGCHandlePtr);

							auto secondNative = nativeWrapper.AsNative<LifetimeTestObject>();
							Assert::NotNull(secondNative.Get());
							Assert::True(firstNative.Get() == secondNative.Get());
						}

						object.Release();
						Assert::Equal(0, LifetimeTestObject::GetDestroyedCount());

						disposeHeld();
						Assert::Equal(1, LifetimeTestObject::GetDestroyedCount());

						return Task<>::CompletedTask();
					}
				}
			}
		};
	}
}
