// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/LanguageSupportMacros.h"

export module Core:Version;

export import :IntegralTypes;
export import :String;

export struct CORE_API Version
{
	int32 Major;
	int32 Minor;
	int32 Build;
	int32 Revision;

	constexpr Version() noexcept
		: Major(0)
		, Minor(0)
		, Build(0)
		, Revision(0)
	{
	}

	Version(String InVersionStr);

	constexpr Version(int32 InMajor, int32 InMinor, int32 InBuild = 0, int32 InRevision = 0) noexcept
		: Major(InMajor)
		, Minor(InMinor)
		, Build(InBuild)
		, Revision(InRevision)
	{
	}

	constexpr Version(const Version& Rhs) noexcept
		: Major(Rhs.Major)
		, Minor(Rhs.Minor)
		, Build(Rhs.Build)
		, Revision(Rhs.Revision)
	{
	}

	String ToString() const
	{
		if (Revision != 0)
		{
			return String::Format(TEXT("{}.{}.{}.{}"), Major, Minor, Build, Revision);
		}
		else if (Build != 0)
		{
			return String::Format(TEXT("{}.{}.{}"), Major, Minor, Build);
		}
		else
		{
			return String::Format(TEXT("{}.{}"), Major, Minor);
		}
	}

	static bool TryParse(String InVersionStr, Version& OutVersion);

	constexpr auto operator <=>(const Version& Rhs) const noexcept
	{
		if (auto V1 = Major <=> Rhs.Major; V1 != 0)
		{
			return V1;
		}
		else if (auto V2 = Minor <=> Rhs.Minor; V2 != 0)
		{
			return V2;
		}
		else if (auto V3 = Build <=> Rhs.Build; V3 != 0)
		{
			return V3;
		}
		else if (auto V4 = Revision <=> Rhs.Major; V4 != 0)
		{
			return V4;
		}
		return std::strong_ordering::equal;
	}

	constexpr auto operator ==(const Version& Rhs) const noexcept
	{
		return Major == Rhs.Major
			&& Minor == Rhs.Minor
			&& Build == Rhs.Build
			&& Revision == Rhs.Revision;
	}

	constexpr Version& operator =(const Version& Rhs) noexcept
	{
		Major = Rhs.Major;
		Minor = Rhs.Minor;
		Build = Rhs.Build;
		Revision = Rhs.Revision;
		return *this;
	}
};