// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Marshal/NativeExceptionInterop.h"
#include "Exception.h"
#include "ManagedException.h"
#include <atomic>
#include <mutex>
#include <typeinfo>
#include <unordered_map>
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
			uint64 m_ExceptionToken = 0;
			uint64 m_ManagedExceptionToken = 0;
		};

		thread_local LastNativeException GLastNativeException;
		std::atomic<uint64> GNextNativeExceptionToken = 1;
		std::mutex GNativeExceptionsMutex;
		std::unordered_map<uint64, std::exception_ptr> GNativeExceptions;

		uint64 RegisterException(std::exception_ptr exception) noexcept
		{
			if (exception == nullptr)
			{
				return 0;
			}

			try
			{
				uint64 exceptionToken = GNextNativeExceptionToken.fetch_add(1, std::memory_order_relaxed);
				if (exceptionToken == 0)
				{
					exceptionToken = GNextNativeExceptionToken.fetch_add(1, std::memory_order_relaxed);
				}

				std::scoped_lock lock(GNativeExceptionsMutex);
				GNativeExceptions.emplace(exceptionToken, std::move(exception));
				return exceptionToken;
			}
			catch (...)
			{
				return 0;
			}
		}

		void StoreException(
			String typeName,
			String message,
			String details,
			String sourceFile = {},
			String sourceFunction = {},
			int32 sourceLine = 0,
			uint64 exceptionToken = 0,
			uint64 managedExceptionToken = 0) noexcept
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
				GLastNativeException.m_ExceptionToken = exceptionToken;
				GLastNativeException.m_ManagedExceptionToken = managedExceptionToken;
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
				GLastNativeException.m_ExceptionToken = 0;
				GLastNativeException.m_ManagedExceptionToken = managedExceptionToken;
			}
		}

		void StoreManagedException(const ManagedException& exception, uint64 exceptionToken) noexcept
		{
			try
			{
				StoreException(
					exception.GetManagedTypeName(),
					exception.GetMessage(),
					exception.GetManagedDetails(),
					{},
					{},
					0,
					exceptionToken,
					exception.GetManagedExceptionToken());
			}
			catch (...)
			{
				StoreException(
					TEXT("System.Exception"),
					TEXT("Failed to capture managed exception details."),
					TEXT("Failed to capture managed exception details."),
					{},
					{},
					0,
					exceptionToken,
					exception.GetManagedExceptionToken());
			}
		}

		void StoreAylaException(const Exception& exception, uint64 exceptionToken) noexcept
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
					static_cast<int32>(source.line()),
					exceptionToken);
			}
			catch (...)
			{
				StoreException(
					TEXT("Ayla::Exception"),
					TEXT("Failed to capture Ayla native exception details."),
					TEXT("Failed to capture Ayla native exception details."),
					{},
					{},
					0,
					exceptionToken,
					0);
			}
		}

		void StoreStdException(const std::exception& exception, uint64 exceptionToken) noexcept
		{
			try
			{
				auto typeName = String::FromCodepage(typeid(exception).name());
				auto message = String::FromCodepage(exception.what());
				StoreException(typeName, message, String::Format(TEXT("{}: {}"), typeName, message), {}, {}, 0, exceptionToken);
			}
			catch (...)
			{
				StoreException(
					TEXT("std::exception"),
					TEXT("Failed to capture standard native exception details."),
					TEXT("Failed to capture standard native exception details."),
					{},
					{},
					0,
					exceptionToken,
					0);
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
		catch (const ManagedException& e)
		{
			uint64 exceptionToken = e.GetManagedExceptionToken() == 0 ? RegisterException(exception) : 0;
			StoreManagedException(e, exceptionToken);
		}
		catch (const Exception& e)
		{
			uint64 exceptionToken = RegisterException(exception);
			StoreAylaException(e, exceptionToken);
		}
		catch (const std::exception& e)
		{
			uint64 exceptionToken = RegisterException(exception);
			StoreStdException(e, exceptionToken);
		}
		catch (...)
		{
			uint64 exceptionToken = RegisterException(exception);
			StoreException(
				TEXT("Unknown native exception"),
				TEXT("An unknown native exception was thrown."),
				TEXT("An unknown native exception was thrown."),
				{},
				{},
				0,
				exceptionToken,
				0);
		}

		return NativeCallStatus::Exception;
	}

	std::exception_ptr NativeExceptionInterop::GetCapturedException(uint64 exceptionToken) noexcept
	{
		if (exceptionToken == 0)
		{
			return nullptr;
		}

		try
		{
			std::scoped_lock lock(GNativeExceptionsMutex);
			auto it = GNativeExceptions.find(exceptionToken);
			return it == GNativeExceptions.end() ? nullptr : it->second;
		}
		catch (...)
		{
			return nullptr;
		}
	}

	void NativeExceptionInterop::ReleaseCapturedException(uint64 exceptionToken) noexcept
	{
		if (exceptionToken == 0)
		{
			return;
		}

		try
		{
			std::scoped_lock lock(GNativeExceptionsMutex);
			GNativeExceptions.erase(exceptionToken);
		}
		catch (...)
		{
		}
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
			.m_SourceLine = GLastNativeException.m_SourceLine,
			.m_ExceptionToken = GLastNativeException.m_ExceptionToken,
			.m_ManagedExceptionToken = GLastNativeException.m_ManagedExceptionToken
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

	PLATFORM_SHARED_EXPORT void Ayla__NativeExceptionInterop__ReleaseCapturedException__Injected(::Ayla::uint64 exceptionToken) noexcept
	{
		::Ayla::NativeExceptionInterop::ReleaseCapturedException(exceptionToken);
	}

	PLATFORM_SHARED_EXPORT void Ayla__NativeExceptionInterop__ClearLastException__Injected()
	{
		::Ayla::NativeExceptionInterop::ClearLastException();
	}
}
