// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <format>
#include <chrono>
#include <sstream>
#include <string_view>

// @see https://en.cppreference.com/w/cpp/chrono/parse to check format strings.
namespace libty::DateTimeFormat
{
	class Json
	{
	public:
		template<class time_point>
		static std::wstring ToString(time_point tp)
		{
			return std::format(L"{:%Y-%m-%dT%H:%M:%S%z}", tp);
		}

		template<class time_point>
		static bool TryParse(std::wstring_view text, time_point& tp)
		{
			std::wistringstream wiss(text);
			return std::chrono::from_stream(wiss, L"%Y-%m-%dT%H:%M:%S%z", tp);
		}
	};

	class File
	{
	public:
		template<class time_point>
		static std::wstring ToString(time_point tp)
		{
			return std::format(L"{:%Y-%m-%d-%H-%M-%S}", tp);
		}

		template<class time_point>
		static bool TryParse(std::wstring_view text, time_point& tp)
		{
			std::wistringstream wiss(text);
			return std::chrono::from_stream(wiss, L"%Y-%m-%d-%H-%M-%S", tp);
		}
	};
}