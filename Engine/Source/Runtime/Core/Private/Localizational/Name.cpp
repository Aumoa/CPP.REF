// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "Platform/PlatformMacros.h"
#include "System/LanguageSupportMacros.h"

export module Core:Name;

export import :Std;
export import :PlatformTypes;
export import :IntegralTypes;
export import :String;
export import :Spinlock;

export class CORE_API Name
{
	int32 Index = 0;
	String CachedDisplayStr;

public:
	constexpr Name() noexcept
		: Index(0)
		, CachedDisplayStr(TEXT("None"))
	{
	}

	constexpr Name(const Name& Rhs) noexcept
		: Index(Rhs.Index)
		, CachedDisplayStr(Rhs.CachedDisplayStr)
	{
	}

	constexpr Name(Name&& Rhs) noexcept
		: Index(Rhs.Index)
		, CachedDisplayStr(std::move(Rhs.CachedDisplayStr))
	{
		Rhs.Index = 0;
		Rhs.CachedDisplayStr = TEXT("None");
	}

	Name(String Str);

	constexpr String ToString() const noexcept
	{
		return CachedDisplayStr;
	}

	constexpr auto operator ==(const Name& Rhs) const noexcept
	{
		return Index == Rhs.Index;
	}

	constexpr auto operator <=>(const Name& Rhs) const noexcept
	{
		return Index <=> Rhs.Index;
	}

	constexpr Name& operator =(const Name& Rhs) noexcept
	{
		Index = Rhs.Index;
		CachedDisplayStr = Rhs.CachedDisplayStr;
		return *this;
	}

	constexpr Name& operator =(Name&& Rhs) noexcept
	{
		Index = Rhs.Index;
		CachedDisplayStr = std::move(Rhs.CachedDisplayStr);
		Rhs.Index = 0;
		Rhs.CachedDisplayStr = TEXT("None");
		return *this;
	}

	Name& operator =(String Str)
	{
		Index = IndexOf(Str);
		CachedDisplayStr = Str;
		return *this;
	}

	static constexpr Name None() noexcept
	{
		return Name();
	}

private:
	static int32 IndexOf(String Str);
	static int32 AllocateNameInternal(String Str);

	struct NameStringComparator
	{
		constexpr bool operator ()(const String& Lhs, const String& Rhs) const noexcept
		{
			return Lhs.CompareTo(Rhs, EStringComparison::CurrentCultureIgnoreCase) < 0;
		}
	};

	static std::vector<String>& IndexCollection() noexcept;
	static std::map<String, int32, NameStringComparator>& IndexMap() noexcept;
	static std::unique_lock<Spinlock> Lock();
};