// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:DateTimeFormat_Localization;

export import :StaticClass;
export import :String;

export class DateTime;

export namespace DateTimeFormat
{
	struct CORE_API Localization : public StaticClass
	{
		static String ToString(const DateTime& Time);
	};
}