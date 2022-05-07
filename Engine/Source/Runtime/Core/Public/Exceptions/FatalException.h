// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/StringView.h"
#include "Stacktrace/Stacktrace.h"
#include <exception>
#include <string>
#include <string_view>
#include <source_location>

namespace libty::inline Core
{
	class CORE_API FatalException : public std::exception
	{
		struct _Impl_buf
		{
			bool _cached;
			String _description;
			String _stacktrace;
			String _fulltrace;
			std::string _what;
		};

		Stacktrace _stacktrace;
		String _message;
		std::exception_ptr _innerException;
		std::shared_ptr<_Impl_buf> _impl_buf;

	public:
		FatalException(StringView message = TEXT("An exception was thrown."), std::exception_ptr innerException = {}) noexcept;
		FatalException(const FatalException& rhs) noexcept;
		FatalException(FatalException&& rhs) noexcept;
		virtual ~FatalException() noexcept;

		virtual const char* what() const noexcept override;
		virtual StringView ToString() const noexcept;

		virtual const Stacktrace& GetStacktrace() const noexcept;
		virtual StringView GetMessage() const noexcept;
		virtual std::exception_ptr GetInnerException() const noexcept;

		static FatalException* AvailableException() noexcept;

	private:
		void _Cache_strings() const noexcept;
	};
}
