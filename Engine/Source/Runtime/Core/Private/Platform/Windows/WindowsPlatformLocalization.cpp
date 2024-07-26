// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:WindowsPlatformLocalization;

export import :Std;
export import :GenericPlatformLocalization;
export import :IntegralTypes;

export class CORE_API WindowsPlatformLocalization : public GenericPlatformLocalization
{
public:
	static std::wstring FromCodepage(std::string_view Str, int32 InCodepage = 0);
	static std::string ToCodepage(std::wstring_view Wstr, int32 InCodepage = 0);
};