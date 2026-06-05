// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Marshal/NativeExceptionInterop.h"
#include "Exception.h"
#include <typeinfo>
#include <utility>

namespace Ayla
{
	namespace
	{
		struct LastNativeException
		{
			bool m_HasException = false;
			String m_TypeName;
			String m_Message;
			String m_Details;
			String m_SourceFile;
			String m_SourceFunction;
			int32 m_SourceLine = 0;
		};

		thread_local LastNativeException GLastNativeException;

		void StoreException(
			String typeName,
			String message,
			String details,
			String sourceFile = {},
			String sourceFunction = {},
			int32 sourceLine = 0) noexcept
		{
			try
			{
				GLastNativeException.m_HasException = true;
				GLastNativeException.m_TypeName = std::move(typeName);
				GLastNativeException.m_Message = std::move(message);
				GLastNativeException.m_Details = std::move(details);
				GLastNativeException.m_SourceFile = std::move(sourceFile);
				GLastNativeException.m_SourceFunction = std::move(sourceFunction);
				GLastNativeException.m_SourceLine = sourceLine;
			}
			catch (...)
			{
				GLastNativeException.m_HasException = true;
				GLastNativeException.m_TypeName = TEXT("Unknown native exception");
				GLastNativeException.m_Message = TEXT("Failed to capture native exception details.");
				GLastNativeException.m_Details = GLastNativeException.m_Message;
				GLastNativeException.m_SourceFile = {};
				GLastNativeException.m_SourceFunction = {};
				GLastNativeException.m_SourceLine = 0;
			}
		}

		void StoreAylaException(const Exception& exception) noexcept
		{
			try
			{
				auto source = exception.GetSourceLocation();
				StoreException(
					String::FromCodepage(typeid(exception).name()),
					exception.GetMessage(),
					exception.ToString(),
					String::FromCodepage(source.file_name()),
					String::FromCodepage(source.function_name()),
					static_cast<int32>(source.line()));
			}
			catch (...)
			{
				StoreException(
					TEXT("Ayla::Exception"),
					TEXT("Failed to capture Ayla native exception details."),
					TEXT("Failed to capture Ayla native exception details."));
			}
		}

		void StoreStdException(const std::exception& exception) noexcept
		{
			try
			{
				auto typeName = String::FromCodepage(typeid(exception).name());
				auto message = String::FromCodepage(exception.what());
				StoreException(typeName, message, String::Format(TEXT("{}: {}"), typeName, message));
			}
			catch (...)
			{
				StoreException(
					TEXT("std::exception"),
					TEXT("Failed to capture standard native exception details."),
					TEXT("Failed to capture standard native exception details."));
			}
		}

		ManagedStringWrapper ToManagedString(String value) noexcept
		{
			try
			{
				return ManagedStringWrapper::FromIntString(std::move(value));
			}
			catch (...)
			{
				try
				{
					return ManagedStringWrapper::FromIntString(TEXT("Failed to marshal native exception text."));
				}
				catch (...)
				{
					return {};
				}
			}
		}
	}

	NativeCallStatus NativeExceptionInterop::CaptureCurrentException() noexcept
	{
		return CaptureException(std::current_exception());
	}

	NativeCallStatus NativeExceptionInterop::CaptureException(std::exception_ptr exception) noexcept
	{
		try
		{
			if (exception)
			{
				std::rethrow_exception(exception);
			}

			StoreException(
				TEXT("Unknown native exception"),
				TEXT("No active native exception was captured."),
				TEXT("No active native exception was captured."));
		}
		catch (const Exception& e)
		{
			StoreAylaException(e);
		}
		catch (const std::exception& e)
		{
			StoreStdException(e);
		}
		catch (...)
		{
			StoreException(
				TEXT("Unknown native exception"),
				TEXT("An unknown native exception was thrown."),
				TEXT("An unknown native exception was thrown."));
		}

		return NativeCallStatus::Exception;
	}

	NativeExceptionInfo NativeExceptionInterop::GetLastException() noexcept
	{
		if (GLastNativeException.m_HasException == false)
		{
			StoreException(
				TEXT("Unknown native exception"),
				TEXT("No native exception details are available."),
				TEXT("No native exception details are available."));
		}

		return NativeExceptionInfo
		{
			.m_TypeName = ToManagedString(GLastNativeException.m_TypeName),
			.m_Message = ToManagedString(GLastNativeException.m_Message),
			.m_Details = ToManagedString(GLastNativeException.m_Details),
			.m_SourceFile = ToManagedString(GLastNativeException.m_SourceFile),
			.m_SourceFunction = ToManagedString(GLastNativeException.m_SourceFunction),
			.m_SourceLine = GLastNativeException.m_SourceLine
		};
	}

	void NativeExceptionInterop::ClearLastException() noexcept
	{
		GLastNativeException = {};
	}
}

extern "C"
{
	PLATFORM_SHARED_EXPORT ::Ayla::NativeExceptionInfo Ayla__NativeExceptionInterop__GetLastException__Injected()
	{
		return ::Ayla::NativeExceptionInterop::GetLastException();
	}

	PLATFORM_SHARED_EXPORT void Ayla__NativeExceptionInterop__ClearLastException__Injected()
	{
		::Ayla::NativeExceptionInterop::ClearLastException();
	}
}
