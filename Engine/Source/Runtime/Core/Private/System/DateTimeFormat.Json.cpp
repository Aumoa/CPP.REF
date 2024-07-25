// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:DateTimeFormat_Json;

export import :Forward;
export import :String;
export import :StaticClass;

export namespace DateTimeFormat
{
	struct CORE_API Json : public StaticClass
	{
		static String ToString(const DateTime& Time);
	};
}