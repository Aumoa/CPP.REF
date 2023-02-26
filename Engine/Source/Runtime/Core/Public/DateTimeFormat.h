// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreTypes/String.h"
#include <chrono>
#include <sstream>

// @see https://en.cppreference.com/w/cpp/chrono/parse to check format strings.
namespace DateTimeFormat
{
	class Json
	{
	public:
		template<class time_point>
		static String ToString(time_point tp)
		{
			return String::Format(TEXT("{:%Y-%m-%dT%H:%M:%S%z}"), tp);
		}

		template<class time_point>
		static bool TryParse(const String& text, time_point& tp)
		{
			std::wistringstream wiss((std::wstring)text);
			return std::chrono::from_stream(wiss, TEXT("%Y-%m-%dT%H:%M:%S%z"), tp);
		}
	};

	class File
	{
	public:
		template<class time_point>
		static String ToString(time_point tp)
		{
			return String::Format(TEXT("{:%Y-%m-%d-%H-%M-%S}"), tp);
		}

		template<class time_point>
		static bool TryParse(const String& text, time_point& tp)
		{
			std::wistringstream wiss((std::wstring)text);
			return std::chrono::from_stream(wiss, TEXT("%Y-%m-%d-%H-%M-%S"), tp);
		}
	};
}