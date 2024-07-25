// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:DateTimeFormat_File;

export import :Forward;
export import :String;
export import :StaticClass;

export namespace DateTimeFormat
{
	struct CORE_API File : public StaticClass
	{
		static String ToString(const DateTime& Time);
	};
}