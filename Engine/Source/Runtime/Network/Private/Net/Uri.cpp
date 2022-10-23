// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Net/Uri.h"
#include "Exceptions/FormatException.h"

Uri::Uri()
{
}

Uri::Uri(const String& uri)
	: _uri(uri)
{
	size_t indexOf = 0;
	if (uri.StartsWith(TEXT("http")))
	{
		const bool secured = uri.length() > 4 && uri[4] == 's';
		_protocol = secured ? EHttpProtocol::Secured : EHttpProtocol::None;
		indexOf += (size_t)4 + (secured ? 1 : 0);
	}

	if (_protocol != EHttpProtocol::None)
	{
		const size_t separator = 3;  // prot://
		if (uri.length() < indexOf + separator)
		{
			throw FormatException(TEXT("Invalid HTTP uri provided."));
		}

		indexOf += separator;
	}

	// end of HTTP, separator of port, path separator.
	size_t hostEnd = uri.IndexOfAny(std::array{ ':', '/' }, indexOf);
	if (hostEnd == -1)
	{
		_host = uri.Substring(indexOf);
		return;
	}

	_host = uri.Substring(indexOf, hostEnd - indexOf);
	wchar_t s = uri[hostEnd];
	indexOf = hostEnd + 1;

	if (s == ':')
	{
		size_t atPath = uri.IndexOf('/', indexOf);
		if (atPath == -1)
		{
			BindPort(uri.SubstringView(indexOf));
			return;
		}
		else
		{
			BindPort(uri.SubstringView(indexOf, atPath - indexOf));
			indexOf = atPath;
		}
	}

	if (uri.length() < indexOf)
	{
		return;
	}

	indexOf++;
	_path = uri.Substring(indexOf);
}

inline void Uri::BindPort(std::wstring_view port)
{
	_port = String::Stoi(port);
}