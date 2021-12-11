// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Guid.h"
#include "PlatformMisc/PlatformMacros.h"

#if PLATFORM_WINDOWS

#undef interface
#include <rpc.h>
#undef GetObject
#pragma comment(lib, "Rpcrt4.lib")

std::wstring Guid::ToString() const
{
	RPC_WSTR Str;
	RPC_STATUS S = UuidToStringW(reinterpret_cast<const UUID*>(this), &Str);
	check(S == RPC_S_OK);

	std::wstring Wstr = (const wchar_t*)Str;
	S = RpcStringFreeW(&Str);
	check(S == RPC_S_OK);

	return Wstr;
}

Guid Guid::FromString(std::wstring_view InGuidFormat)
{
	Guid V;
	RPC_STATUS S = UuidFromStringW((RPC_WSTR)InGuidFormat.data(), reinterpret_cast<UUID*>(&V));
	check(S == RPC_S_OK);
	return V;
}

Guid Guid::NewGuid()
{
	Guid V;
	RPC_STATUS S = UuidCreate(reinterpret_cast<UUID*>(&V));
	check(S == RPC_S_OK);
	return V;
}

#else
#error Unsupported platform for Guid.
#endif