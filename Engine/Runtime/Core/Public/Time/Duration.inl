// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "Duration.h"

template<size_t N>
consteval TimeStringLit<N>::TimeStringLit(const char(&text)[N]) : TimeStringLit(text, std::make_index_sequence<N>{ })
{

}

template<size_t N>
template<size_t... Seq>
consteval TimeStringLit<N>::TimeStringLit(const char(&text)[N], std::index_sequence<Seq...>&&)
	: C_Str{ text[Seq]... }
{

}

inline Nanoseconds::Nanoseconds()
{

}

inline constexpr Nanoseconds::Nanoseconds(int64 value) : Unit(Mychrono(value))
{

}

template<class T>
inline constexpr Nanoseconds::Nanoseconds(std::chrono::duration<int64, T> value) : Unit(value)
{

}

inline constexpr bool Nanoseconds::Equals(const Nanoseconds& rh) const
{
	return *this == rh;
}

inline TRefPtr<String> Nanoseconds::ToString() const
{
	return String::Format("{0}ns", Unit.count());
}

inline constexpr int64 Nanoseconds::Value_get() const
{
	return Unit.count();
}

inline void Nanoseconds::Value_set(int64 value)
{
	Unit = Mychrono(value);
}

inline constexpr bool Nanoseconds::operator ==(const Nanoseconds& rh) const
{
	return Unit == rh.Unit;
}

inline constexpr bool Nanoseconds::operator !=(const Nanoseconds& rh) const
{
	return Unit != rh.Unit;
}

inline constexpr bool Nanoseconds::operator < (const Nanoseconds& rh) const
{
	return Unit < rh.Unit;
}

inline constexpr bool Nanoseconds::operator <=(const Nanoseconds& rh) const
{
	return Unit <= rh.Unit;
}

inline constexpr bool Nanoseconds::operator > (const Nanoseconds& rh) const
{
	return Unit > rh.Unit;
}

inline constexpr bool Nanoseconds::operator >=(const Nanoseconds& rh) const
{
	return Unit >= rh.Unit;
}

inline constexpr std::weak_ordering Nanoseconds::operator <=>(const Nanoseconds& rh) const
{
	return Unit.count() <=> rh.Unit.count();
}

template<TIsDuration T>
inline constexpr Nanoseconds::operator T () const
{
	return T(Unit.count());
}

template<TIsRatio TRatio, TimeStringLit TimeStr>
inline constexpr Subseconds<TRatio, TimeStr>::Subseconds()
{

}

template<TIsRatio TRatio, TimeStringLit TimeStr>
inline constexpr Subseconds<TRatio, TimeStr>::Subseconds(int64 nano) : Super(nano)
{

}

template<TIsRatio TRatio, TimeStringLit TimeStr>
inline constexpr Subseconds<TRatio, TimeStr>::Subseconds(float value) : Super(std::chrono::duration_cast<Super::Mychrono>(Mychrono(value)).count())
{

}

template<TIsRatio TRatio, TimeStringLit TimeStr>
inline constexpr Subseconds<TRatio, TimeStr>::Subseconds(Super::Mychrono value) : Super(value)
{

}

template<TIsRatio TRatio, TimeStringLit TimeStr>
inline constexpr Subseconds<TRatio, TimeStr>::Subseconds(std::chrono::duration<int64, TRatio> value) : Super(std::chrono::duration_cast<Super::Mychrono>(value).count())
{

}

template<TIsRatio TRatio, TimeStringLit TimeStr>
inline TRefPtr<String> Subseconds<TRatio, TimeStr>::ToString() const
{
	return String::Format("{0}{1}", Value, TimeStr.C_Str);
}

template<TIsRatio TRatio, TimeStringLit TimeStr>
inline constexpr float Subseconds<TRatio, TimeStr>::Value_get() const
{
	return std::chrono::duration_cast<Mychrono>(Unit).count();
}

template<TIsRatio TRatio, TimeStringLit TimeStr>
inline void Subseconds<TRatio, TimeStr>::Value_set(float value)
{
	Unit = std::chrono::duration_cast<Super::Mychrono>(Mychrono(value));
}