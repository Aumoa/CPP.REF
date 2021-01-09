// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#include <chrono>
#include <compare>
#include "TRefPtr.h"

struct Nanoseconds;

template<class T>
concept TIsDuration = requires(T* Derived, Nanoseconds* Super)
{
	{ Super = Derived };
};

template<class T>
concept TIsRatio = requires()
{
	{ T::den };
	{ T::num };
};

template<size_t N>
struct TimeStringLit
{
	char C_Str[N];

	consteval TimeStringLit(const char(&text)[N]);
	template<size_t... Seq>
	consteval TimeStringLit(const char(&text)[N], std::index_sequence<Seq...>&&);
};

struct Nanoseconds
{
	using This = Nanoseconds;
	using Mychrono = std::chrono::nanoseconds;

	Mychrono Unit;

	inline Nanoseconds();
	inline constexpr Nanoseconds(int64 value);
	inline constexpr Nanoseconds(Mychrono value);

	inline constexpr bool Equals(const Nanoseconds& rh) const;
	inline TRefPtr<String> ToString() const;

	vs_property(int64, Value);
	inline constexpr int64 Value_get() const;
	inline void Value_set(int64 value);

	inline constexpr bool operator ==(const Nanoseconds& rh) const;
	inline constexpr bool operator !=(const Nanoseconds& rh) const;
	inline constexpr bool operator < (const Nanoseconds& rh) const;
	inline constexpr bool operator <=(const Nanoseconds& rh) const;
	inline constexpr bool operator > (const Nanoseconds& rh) const;
	inline constexpr bool operator >=(const Nanoseconds& rh) const;
	inline constexpr std::weak_ordering operator <=>(const Nanoseconds& rh) const;

	template<TIsDuration T>
	inline constexpr operator T () const;
};

template<TIsRatio TRatio, TimeStringLit TimeStr>
struct Subseconds : public Nanoseconds
{
	using Super = Nanoseconds;
	using This = Subseconds;
	using Mychrono = std::chrono::duration<float, TRatio>;

	inline constexpr Subseconds();
	inline constexpr Subseconds(int64 nano);
	inline constexpr Subseconds(float value);
	inline constexpr Subseconds(std::chrono::duration<int64, TRatio> value);
	inline constexpr Subseconds(Super::Mychrono value);

	inline TRefPtr<String> ToString() const;

	vs_property(float, Value);
	inline constexpr float Value_get() const;
	inline void Value_set(float value);
};

#include "Duration.inl"

using Microseconds = Subseconds<std::micro, "μs">;
using Milliseconds = Subseconds<std::milli, "ms">;
using Seconds = Subseconds<std::ratio<1>, "s">;
using Minutes = Subseconds<std::ratio<60>, "m">;
using Hours = Subseconds<std::ratio<3600>, "h">;
using Days = Subseconds<std::ratio<86400>, "d">;