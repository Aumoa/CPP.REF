// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Sockets
{
	class SOCKETS_API SocketException : public FatalException
	{
		std::string _message;
		std::source_location _location;
		std::string _what;

	public:
		SocketException(std::string_view message, std::exception_ptr innerException = nullptr, std::source_location location = std::source_location::current());
	};
}