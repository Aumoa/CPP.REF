// Copyright 2020-2022 Aumoa.lib. All right reserved.

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
	checkf(S == RPC_S_OK, L"Internal RPC failed. ErrorCode: {}", S);

	std::wstring Wstr = (const wchar_t*)Str;
	S = RpcStringFreeW(&Str);
	checkf(S == RPC_S_OK, L"Internal RPC failed. ErrorCode: {}", S);

	return Wstr;
}

Guid Guid::FromString(std::wstring_view InGuidFormat)
{
	Guid V;
	RPC_STATUS S = UuidFromStringW((RPC_WSTR)InGuidFormat.data(), reinterpret_cast<UUID*>(&V));
	checkf(S == RPC_S_OK, L"Internal RPC failed. ErrorCode: {}", S);
	return V;
}

Guid Guid::NewGuid()
{
	Guid V;
	RPC_STATUS S = UuidCreate(reinterpret_cast<UUID*>(&V));
	checkf(S == RPC_S_OK, L"Internal RPC failed. ErrorCode: {}", S);
	return V;
}

#else
#error Unsupported platform for Guid.
#endif