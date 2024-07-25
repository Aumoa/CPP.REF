// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:WindowsPlatformMisc;

export import :GenericPlatformMisc;
export import :String;
export import :IntegralTypes;

export class CORE_API WindowsPlatformMisc : public GenericPlatformMisc
{
public:
	static int32 GetLastError() noexcept;
	static String FormatSystemCode(int32 ErrorCode) noexcept;
};