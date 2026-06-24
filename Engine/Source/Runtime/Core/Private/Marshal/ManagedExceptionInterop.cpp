// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Marshal/ManagedExceptionInterop.h"
#include "Marshal/CoreCLRFunctions.h"
#include "ManagedException.h"
#include <utility>

namespace Ayla
{
	namespace
	{
		struct LastManagedException
		{
			bool m_HasException = false;
			String m_TypeName;
			String m_Message;
			String m_Details;
			std::exception_ptr m_NativeException;
			uint64 m_ManagedExceptionToken = 0;
		};

		thread_local LastManagedException GLastManagedException;
		thread_local bool GIsReleasingManagedExceptionToken = false;

		String CopyManagedString(ManagedStringWrapper value, String fallback) noexcept
		{
			try
			{
				return value.AsString();
			}
			catch (...)
			{
				return fallback;
			}
		}
	}

	NativeCallStatus ManagedExceptionInterop::CaptureException(
		ManagedStringWrapper typeName,
		ManagedStringWrapper message,
		ManagedStringWrapper details,
		uint64 nativeExceptionToken,
		uint64 managedExceptionToken) noexcept
	{
		try
		{
			GLastManagedException.m_HasException = true;
			GLastManagedException.m_TypeName = CopyManagedString(typeName, TEXT("System.Exception"));
			GLastManagedException.m_Message = CopyManagedString(message, TEXT("Failed to capture managed exception message."));
			GLastManagedException.m_Details = CopyManagedString(details, GLastManagedException.m_Message);
			GLastManagedException.m_NativeException = NativeExceptionInterop::GetCapturedException(nativeExceptionToken);
			GLastManagedException.m_ManagedExceptionToken = managedExceptionToken;
		}
		catch (...)
		{
			GLastManagedException.m_HasException = true;
			GLastManagedException.m_TypeName = TEXT("System.Exception");
			GLastManagedException.m_Message = TEXT("Failed to capture managed exception details.");
			GLastManagedException.m_Details = GLastManagedException.m_Message;
			GLastManagedException.m_NativeException = nullptr;
			GLastManagedException.m_ManagedExceptionToken = managedExceptionToken;
		}

		return NativeCallStatus::Exception;
	}

	void ManagedExceptionInterop::ReleaseCapturedException(uint64 managedExceptionToken) noexcept
	{
		if (managedExceptionToken == 0)
		{
			return;
		}
		if (GIsReleasingManagedExceptionToken)
		{
			return;
		}

		GIsReleasingManagedExceptionToken = true;
		try
		{
			EnsureCoreCLRFunctionsInitialized();
			if (g_CoreCLRFunctions.m_ReleaseManagedExceptionPtr__Invoke != nullptr)
			{
				g_CoreCLRFunctions.m_ReleaseManagedExceptionPtr__Invoke(managedExceptionToken);
			}
		}
		catch (...)
		{
		}
		GIsReleasingManagedExceptionToken = false;
	}

	void ManagedExceptionInterop::ThrowLastException()
	{
		if (GLastManagedException.m_HasException == false)
		{
			throw ManagedException(
				TEXT("System.Exception"),
				TEXT("No managed exception details are available."),
				TEXT("No managed exception details are available."));
		}

		LastManagedException exception = std::move(GLastManagedException);
		GLastManagedException = {};

		if (exception.m_NativeException)
		{
			std::rethrow_exception(exception.m_NativeException);
		}

		throw ManagedException(
			std::move(exception.m_TypeName),
			std::move(exception.m_Message),
			std::move(exception.m_Details),
			exception.m_ManagedExceptionToken);
	}

	void ManagedExceptionInterop::ClearLastException() noexcept
	{
		GLastManagedException = {};
	}
}

extern "C"
{
	PLATFORM_SHARED_EXPORT ::Ayla::NativeCallStatus Ayla__ManagedExceptionInterop__CaptureException__Injected(
		::Ayla::ManagedStringWrapper typeName,
		::Ayla::ManagedStringWrapper message,
		::Ayla::ManagedStringWrapper details,
		::Ayla::uint64 nativeExceptionToken,
		::Ayla::uint64 managedExceptionToken) noexcept
	{
		return ::Ayla::ManagedExceptionInterop::CaptureException(typeName, message, details, nativeExceptionToken, managedExceptionToken);
	}
}
