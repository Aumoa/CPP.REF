// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Tests/ManagedInteropTest.h"
#include "InvalidOperationException.h"
#include "ManagedException.h"
#include "Marshal/ManagedCallBoundary.h"
#include "Marshal/NativeCallBoundary.h"
#include "Marshal/NativeExceptionInterop.h"
#include "ScriptingBackend/ScriptingBackend.h"

namespace Ayla
{
	namespace
	{
		const String NativeExceptionMessage = TEXT("native exception passport");
		const String ManagedExceptionMessage = TEXT("managed exception passport");
		bool GObservedManagedExceptionWrapper = false;
		bool GSwallowedManagedExceptionWrapper = false;

		int32 MultiplyForManagedCallback(int32 left, int32 right)
		{
			return left * right;
		}

		bool IsManagedInteropSmokeException(const ManagedException& exception)
		{
			return exception.GetManagedTypeName().Contains(TEXT("ManagedInteropSmokeException"))
				&& exception.GetMessage() == ManagedExceptionMessage
				&& exception.GetManagedExceptionToken() != 0;
		}

		NativeCallStatus ObserveManagedExceptionThroughNative(ssize_t managedCallback) noexcept
		{
			return NativeCallBoundary::Invoke([&]() -> NativeCallStatus
			{
				using signature_t = NativeCallStatus(*)();
				auto callback = reinterpret_cast<signature_t>(managedCallback);

				try
				{
					ManagedCallBoundary::ThrowIfFailed(callback());
				}
				catch (const ManagedException& ex)
				{
					GObservedManagedExceptionWrapper = IsManagedInteropSmokeException(ex);
					throw;
				}

				return NativeCallStatus::Success;
			});
		}

		NativeCallStatus SwallowManagedExceptionInNative(ssize_t managedCallback) noexcept
		{
			return NativeCallBoundary::Invoke([&]() -> NativeCallStatus
			{
				using signature_t = NativeCallStatus(*)();
				auto callback = reinterpret_cast<signature_t>(managedCallback);

				try
				{
					ManagedCallBoundary::ThrowIfFailed(callback());
				}
				catch (const ManagedException& ex)
				{
					GSwallowedManagedExceptionWrapper = IsManagedInteropSmokeException(ex);
				}

				return NativeCallStatus::Success;
			});
		}

		NativeCallStatus ThrowNativeExceptionCallback() noexcept
		{
			return NativeCallBoundary::Invoke([]() -> NativeCallStatus
			{
				throw InvalidOperationException(NativeExceptionMessage);
			});
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
			},
			TestCase
			{
				.Name = TEXT("Core debug log boundary"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						using signature_t = int32(*)();
						auto function = GetManagedInteropFunction<signature_t>("InvokeCoreDebugLog");

						Assert::Equal(1, function());

						return Task<>::CompletedTask();
					}
				}
			},
			TestCase
			{
				.Name = TEXT("Managed exception restores after native frame"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						using signature_t = int32(*)(ssize_t);
						auto function = GetManagedInteropFunction<signature_t>("RoundTripManagedExceptionThroughNative");

						GObservedManagedExceptionWrapper = false;

						Assert::Equal(1, function(reinterpret_cast<ssize_t>(&ObserveManagedExceptionThroughNative)));
						Assert::True(GObservedManagedExceptionWrapper);

						return Task<>::CompletedTask();
					}
				}
			},
			TestCase
			{
				.Name = TEXT("Managed exception token releases after native swallow"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						using signature_t = int32(*)(ssize_t);
						auto function = GetManagedInteropFunction<signature_t>("MeasureManagedExceptionTokenReleaseAfterNativeSwallow");

						GSwallowedManagedExceptionWrapper = false;

						Assert::Equal(0, function(reinterpret_cast<ssize_t>(&SwallowManagedExceptionInNative)));
						Assert::True(GSwallowedManagedExceptionWrapper);

						return Task<>::CompletedTask();
					}
				}
			},
			TestCase
			{
				.Name = TEXT("Native exception restores after managed frame"),
				.TestFuncs =
				{
					[](std::stop_token)
					{
						using round_trip_signature_t = NativeCallStatus(*)(ssize_t);
						using observed_signature_t = int32(*)();
						auto roundTrip = GetManagedInteropFunction<round_trip_signature_t>("RoundTripNativeExceptionThroughManaged");
						auto getObservedNativeException = GetManagedInteropFunction<observed_signature_t>("GetObservedNativeExceptionWrapper");

						size_t beforeExceptionCount = NativeExceptionInterop::GetCapturedExceptionCount();
						bool restoredNativeException = false;
						try
						{
							ManagedCallBoundary::ThrowIfFailed(roundTrip(reinterpret_cast<ssize_t>(&ThrowNativeExceptionCallback)));
						}
						catch (const InvalidOperationException& ex)
						{
							restoredNativeException = ex.GetMessage() == NativeExceptionMessage;
						}
						catch (const ManagedException& ex)
						{
							Assert::Fail(String::Format(TEXT("Expected restored native exception, got managed wrapper: {}"), ex.GetManagedTypeName()));
						}

						Assert::True(restoredNativeException);
						Assert::Equal(1, getObservedNativeException());
						Assert::Equal(beforeExceptionCount, NativeExceptionInterop::GetCapturedExceptionCount());

						return Task<>::CompletedTask();
					}
				}
			}
		};
	}
}
