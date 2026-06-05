// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Marshal/ManagedExceptionInterop.h"
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
		};

		thread_local LastManagedException GLastManagedException;

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
		ManagedStringWrapper details) noexcept
	{
		try
		{
			GLastManagedException.m_HasException = true;
			GLastManagedException.m_TypeName = CopyManagedString(typeName, TEXT("System.Exception"));
			GLastManagedException.m_Message = CopyManagedString(message, TEXT("Failed to capture managed exception message."));
			GLastManagedException.m_Details = CopyManagedString(details, GLastManagedException.m_Message);
		}
		catch (...)
		{
			GLastManagedException.m_HasException = true;
			GLastManagedException.m_TypeName = TEXT("System.Exception");
			GLastManagedException.m_Message = TEXT("Failed to capture managed exception details.");
			GLastManagedException.m_Details = GLastManagedException.m_Message;
		}

		return NativeCallStatus::Exception;
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

		throw ManagedException(
			std::move(exception.m_TypeName),
			std::move(exception.m_Message),
			std::move(exception.m_Details));
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
		::Ayla::ManagedStringWrapper details) noexcept
	{
		return ::Ayla::ManagedExceptionInterop::CaptureException(typeName, message, details);
	}
}
