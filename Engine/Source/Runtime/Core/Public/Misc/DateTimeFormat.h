// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreTypes/String.h"
#include <chrono>
#include <sstream>

class DateTime;

namespace DateTimeFormat
{
	class CORE_API DateTimeFormatter
	{
		DateTimeFormatter() = delete;

	public:
		using clock = std::chrono::system_clock;
		using time_point = clock::time_point;
		using local_time = std::chrono::local_time<clock::duration>;

	protected:
		static const time_point& GetTimePoint(const DateTime& Time);
		static const local_time& GetLocalTime(const DateTime& Time);
	};

	class CORE_API Localization : public DateTimeFormatter
	{
		Localization() = delete;

	public:
		static String ToString(const DateTime& Time);
	};

	class CORE_API Json : public DateTimeFormatter
	{
		Json() = delete;

	public:
		static String ToString(const DateTime& Time);
	};

	class CORE_API File : public DateTimeFormatter
	{
		File() = delete;

	public:
		static String ToString(const DateTime& Time);
	};
}