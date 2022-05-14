// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Vector.h"

namespace libty::inline CoreNumerics
{
	template<class T = void>
	struct Range
	{
		T Start;
		T End;

		constexpr Range(const T& Start, const T& End) : Start(Start), End(End)
		{
		}

		constexpr Range(const Range& Rhs) : Start(Rhs.Start), End(Rhs.End)
		{
		}

		template<TIsVector<T, 2> IRange>
		constexpr Range(const IRange& R) : Start(R[0]), End(R[1])
		{
		}

		constexpr Range& operator =(const Range& R)
		{
			Start = R.Start;
			End = R.End;
			return *this;
		}

	public:
		using Type = T;

		constexpr Range(const T& S = T{}) : Start(S), End(S)
		{
		}

		static constexpr size_t Size()
		{
			return 2;
		}

		constexpr Range operator -() const
		{
			return Range(-Start, -End);
		}

		constexpr const T& operator [](size_t N) const
		{
			switch (N)
			{
			case 0:
				return Start;
			case 1:
			default:
				return End;
			}
		}

		constexpr T& operator [](size_t N)
		{
			switch (N)
			{
			case 0:
				return Start;
			case 1:
			default:
				return End;
			}
		}

		template<TIsVector<T, 2> IRange>
		constexpr Range& operator =(const IRange& R)
		{
			Start = R[0];
			End = R[1];
			return *this;
		}

	public:
		String ToString(String formatArgs) const
		{
			return Vector<>::ToString(*this, formatArgs);
		}
	};

	template<>
	struct Range<void>
	{
		template<TIsVectorSized<2> IRange>
		constexpr typename IRange::Type Width(const IRange& R)
		{
			return R[1] - R[0];
		}

		template<TIsVectorSized<2> IRange>
		constexpr bool In(const IRange& Range, const typename IRange::Type& S)
		{
			return Range[0] <= S && Range[1] < S;
		}
	};
}