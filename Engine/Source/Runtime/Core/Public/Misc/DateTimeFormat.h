// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "String.h"
#include <chrono>
#include <sstream>
#include <string_view>
#include <version>

#if PLATFORM_LINUX
#include <iomanip>
#endif

namespace libty::inline Core
{
	// @see https://en.cppreference.com/w/cpp/chrono/parse to check format strings.
	namespace DateTimeFormat
	{
		class Json
		{
		public:
			template<class time_point>
			static std::wstring ToString(time_point tp)
			{
	#if PLATFORM_LINUX
				auto tt = time_point::clock::to_time_t(tp);
				std::ostringstream oss;
				oss << std::put_time(std::localtime(&tt), "%Y-%m-%dT%H:%M:%S%z");
				return String::AsUnicode(oss.str());
	#else
				return String::Format(L"{:%Y-%m-%dT%H:%M:%S%z}", tp);
	#endif
			}

			template<class time_point>
			static bool TryParse(std::wstring_view text, time_point& tp)
			{
				std::wistringstream wiss(text);
	#if PLATFORM_LINUX
				wchar_t ignore = 0;
				std::tm tm = {};
				int region = 0;
				wiss >> tm.tm_year >> ignore >> tm.tm_mon >> ignore >> tm.tm_mday >> ignore >> tm.tm_hour >> ignore >> tm.tm_min >> ignore >> tm.tm_sec >> ignore >> region;

				time_t t = timegm(&tm);
				tp = std::chrono::system_clock::from_time_t(t);
				return true;
	#else
				return std::chrono::from_stream(wiss, L"%Y-%m-%dT%H:%M:%S%z", tp);
	#endif
			}
		};

		class File
		{
		public:
			template<class time_point>
			static std::wstring ToString(time_point tp)
			{
	#if PLATFORM_LINUX
				auto tt = time_point::clock::to_time_t(tp);
				std::ostringstream oss;
				oss << std::put_time(std::localtime(&tt), "%Y-%m-%d-%H-%M-%S");
				return String::AsUnicode(oss.str());
	#else
				return String::Format(L"{:%Y-%m-%d-%H-%M-%S}", tp);
	#endif
			}

			template<class time_point>
			static bool TryParse(std::wstring_view text, time_point& tp)
			{
				std::wistringstream wiss(text);
	#if __cpp_lib_chrono && !PLATFORM_LINUX
				return std::chrono::from_stream(wiss, L"%Y-%m-%d-%H-%M-%S", tp);
	#else
				wchar_t ignore = 0;
				std::tm tm = {};
				int region = 0;
				wiss >> tm.tm_year >> ignore >> tm.tm_mon >> ignore >> tm.tm_mday >> ignore >> tm.tm_hour >> ignore >> tm.tm_min >> ignore >> tm.tm_sec;

	#if PLATFORM_LINUX
				time_t t = timegm(&tm);
				tp = std::chrono::system_clock::from_time_t(t);
	#endif
				return true;
	#endif
			}
		};
	}
}