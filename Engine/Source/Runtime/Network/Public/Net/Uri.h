// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Http/HttpProtocol.h"

class NETWORK_API Uri
{
private:
	String _uri;
	
	EHttpProtocol _protocol = EHttpProtocol::None;
	String _host;
	int32 _port = 0;
	String _path;

public:
	Uri();
	Uri(const String& uri);

	inline EHttpProtocol GetProtocol() const noexcept { return _protocol; }
	inline String GetHost() const noexcept { return _host; }
	inline int32 GetPort() const noexcept { return _port; }
	inline String GetPath() const noexcept { return _path; }

private:
	inline void BindPort(std::wstring_view port);
};