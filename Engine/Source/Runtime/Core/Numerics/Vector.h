// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <initializer_list>
#include <string>
#include <span>
#include <array>
#include "Mathematics/MathEx.h"
#include "Utilities/StringUtils.h"

/// <summary>
/// Represents a vector with specified count floating values.
/// </summary>
template<size_t N>
struct Vector
{
	/// <summary>
	/// The values.
	/// </summary>
	float Values[N] = {};

	/// <summary>
	/// Initialize new <see cref="Vector"/> instance.
	/// </summary>
	inline constexpr Vector()
	{
	}

	/// <summary>
	/// Initialize new <see cref="Vector"/> instance.
	/// </summary>
	/// <param name="v"> The initialize value. </param>
	inline constexpr Vector(float v)
	{
		for (size_t i = 0; i < N; ++i)
		{
			Values[i] = v;
		}
	}

	/// <summary>
	/// Initialize new <see cref="Vector"/> instance.
	/// </summary>
	inline constexpr Vector(const Vector<N - 1>& lhs, float rhs)
	{
		for (size_t i = 0; i < N - 1; ++i)
		{
			Values[i] = lhs.Values[i];
		}
		Values[N - 1] = rhs;
	}

	/// <summary>
	/// Initialize new <see cref="Vector"/> instance.
	/// </summary>
	/// <param name="initializer"> The initializer to initialize vector values. </param>
	inline constexpr Vector(std::initializer_list<float> initializer)
	{
		for (size_t i = 0; i < MathEx::Min(initializer.size(), (size_t)N); ++i)
		{
			Values[i] = initializer.begin()[i];
		}
	}

	/// <summary>
	/// Initialize new <see cref="Vector"/> instance.
	/// </summary>
	/// <param name="copy"> The copy vector to initialize vector values. </param>
	inline constexpr Vector(const Vector& copy)
	{
		for (size_t i = 0; i < N; ++i)
		{
			Values[i] = copy[i];
		}
	}

	/// <summary>
	/// Initialize new <see cref="Vector"/> instance.
	/// </summary>
	/// <param name="copy"> The copy vector to initialize vector values. </param>
	/// <param name="fill"> The fill value for placed without initialization. </param>
	template<size_t ON>
	inline constexpr Vector(const Vector<ON>& copy, float fill = 0.0f)
	{
		for (size_t i = 0; i < N; ++i)
		{
			if (i < ON)
			{
				Values[i] = copy[i];
			}
			else
			{
				Values[i] = fill;
			}
		}
	}

	/// <summary>
	/// Get vector element count.
	/// </summary>
	/// <returns> The element count. </returns>
	static inline constexpr size_t Num()
	{
		return N;
	}

	/// <summary>
	/// Get squared length of this vector.
	/// </summary>
	/// <returns> The squared length. </returns>
	inline constexpr float GetLengthSq() const
	{
		float v = 0;
		for (size_t i = 0; i < N; ++i)
		{
			v += Values[i] * Values[i];
		}
		return v;
	}

	/// <summary>
	/// Get length of this vector.
	/// </summary>
	/// <returns> The squared length. </returns>
	inline float GetLength() const
	{
		return MathEx::Sqrt(GetLengthSq());
	}

	/// <summary>
	/// Compare nearly equals to target vector with epsilon value.
	/// </summary>
	/// <param name="rhs"> The target vector. </param>
	/// <param name="epsilon"> The epsilon value. If different of two components is lower than this values, is nearly equals. </param>
	/// <returns> Indicate two vectors is nearly equals. </returns>
	inline constexpr bool NearlyEquals(const Vector& rhs, float epsilon) const
	{
		for (size_t i = 0; i < N; ++i)
		{
			if (MathEx::Abs(Values[i] - rhs.Values[i]) > epsilon)
			{
				return false;
			}
		}
		return true;
	}

	inline constexpr const float& operator [](size_t idx) const
	{
		return Values[idx];
	}

	inline float& operator [](size_t idx)
	{
		return Values[idx];
	}

	inline constexpr bool operator ==(const Vector& rhs) const
	{
		for (size_t i = 0; i < N; ++i)
		{
			if (Values[i] != rhs.Values[i])
			{
				return false;
			}
		}
		return true;
	}

	inline constexpr bool operator !=(const Vector& rhs) const
	{
		for (size_t i = 0; i < N; ++i)
		{
			if (Values[i] == rhs.Values[i])
			{
				return false;
			}
		}
		return true;
	}

	/// <summary>
	/// Get squared distance between two vectors.
	/// </summary>
	/// <param name="lhs"> The first vector. </param>
	/// <param name="rhs"> The second vector. </param>
	/// <returns> The squared distance between two vectors. </returns>
	static inline constexpr float GetDistanceSq(const Vector& lhs, const Vector& rhs)
	{
		return (rhs - lhs).GetLengthSq();
	}

	/// <summary>
	/// Get distance between two vectors.
	/// </summary>
	/// <param name="lhs"> The first vector. </param>
	/// <param name="rhs"> The second vector. </param>
	/// <returns> The distance between two vectors. </returns>
	static inline float GetDistance(const Vector& lhs, const Vector& rhs)
	{
		return (rhs - lhs).GetLength();
	}

	/// <summary>
	/// Get normalized vector.
	/// </summary>
	/// <returns> The normalized vector. </returns>
	inline Vector GetNormal() const
	{
		float invSqrt = MathEx::InvSqrt(GetLengthSq());
		Vector result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Values[i] = Values[i] * invSqrt;
		}
		return result;
	}

	inline constexpr Vector operator -() const
	{
		Vector result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Values[i] = -Values[i];
		}
		return result;
	}

	inline constexpr Vector operator +(const Vector& rhs) const
	{
		Vector result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Values[i] = Values[i] + rhs.Values[i];
		}
		return result;
	}

	inline constexpr Vector operator -(const Vector& rhs) const
	{
		Vector result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Values[i] = Values[i] - rhs.Values[i];
		}
		return result;
	}

	inline constexpr Vector operator *(const Vector& rhs) const
	{
		Vector result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Values[i] = Values[i] * rhs.Values[i];
		}
		return result;
	}

	inline constexpr Vector operator /(const Vector& rhs) const
	{
		Vector result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Values[i] = Values[i] / rhs.Values[i];
		}
		return result;
	}

	inline constexpr Vector operator *(float rhs) const
	{
		Vector result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Values[i] = Values[i] * rhs;
		}
		return result;
	}

	inline constexpr Vector operator /(float rhs) const
	{
		Vector result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Values[i] = Values[i] / rhs;
		}
		return result;
	}

	inline Vector& operator =(const Vector& rhs)
	{
		for (size_t i = 0; i < N; ++i)
		{
			Values[i] = rhs.Values[i];
		}
		return *this;
	}

	inline Vector& operator +=(const Vector& rhs)
	{
		for (size_t i = 0; i < N; ++i)
		{
			Values[i] += rhs.Values[i];
		}
		return *this;
	}

	inline Vector& operator -=(const Vector& rhs)
	{
		for (size_t i = 0; i < N; ++i)
		{
			Values[i] -= rhs.Values[i];
		}
		return *this;
	}

	inline Vector& operator *=(const Vector& rhs)
	{
		for (size_t i = 0; i < N; ++i)
		{
			Values[i] *= rhs.Values[i];
		}
		return *this;
	}

	inline Vector& operator /=(const Vector& rhs)
	{
		for (size_t i = 0; i < N; ++i)
		{
			Values[i] /= rhs.Values[i];
		}
		return *this;
	}

	inline Vector& operator *=(float rhs)
	{
		for (size_t i = 0; i < N; ++i)
		{
			Values[i] *= rhs;
		}
		return *this;
	}

	inline Vector& operator /=(float rhs)
	{
		for (size_t i = 0; i < N; ++i)
		{
			Values[i] /= rhs;
		}
		return *this;
	}

	/// <summary>
	/// Get vector composed of each maximum components.
	/// </summary>
	/// <param name="lhs"> The first vector. </param>
	/// <param name="rhs"> The second vector.</param>
	/// <returns> The result vector. </returns>
	static inline constexpr Vector Max(const Vector& lhs, const Vector& rhs)
	{
		Vector result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Values[i] = MathEx::Max(lhs.Values[i], rhs.Values[i]);
		}
		return result;
	}

	/// <summary>
	/// Get vector composed of each minimum components.
	/// </summary>
	/// <param name="lhs"> The first vector. </param>
	/// <param name="rhs"> The second vector.</param>
	/// <returns> The result vector. </returns>
	static inline constexpr Vector Min(const Vector& lhs, const Vector& rhs)
	{
		Vector result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Values[i] = MathEx::Min(lhs.Values[i], rhs.Values[i]);
		}
		return result;
	}

	/// <summary>
	/// Get simple string represents this vector value.
	/// </summary>
	/// <param name="formatArgs"> The formatting args that use to std::format. </param>
	/// <returns> The simple string value. </returns>
	inline std::wstring ToString(std::wstring_view formatArgs = L"") const
	{
		std::wstring placeholder = StringUtils::GetPlaceholder(formatArgs);

		std::array<std::wstring, N> composed;
		for (size_t i = 0; i < N; ++i)
		{
			composed[i] = format(placeholder, Values[i]);
		}

		return format(L"{{{}}}", StringUtils::Join(L", ", std::span<std::wstring const>(composed)));
	}

	/// <summary>
	/// Swizzling vector that compose components specified indexes.
	/// </summary>
	/// <returns> Composed vector. </returns>
	template<size_t... Indexes>
	inline constexpr Vector<sizeof...(Indexes)> Swiz() const
	{
		return Vector<sizeof...(Indexes)>{ Values[Indexes]... };
	}

	/// <summary>
	/// Get zero vector.
	/// </summary>
	inline static constexpr Vector GetZero();
};

template<size_t N>
inline constexpr Vector<N> operator *(float lhs, const Vector<N>& rhs)
{
	return Vector<N>(lhs) * rhs;
}

template<size_t N>
inline constexpr Vector<N> operator /(float lhs, const Vector<N>& rhs)
{
	return Vector<N>(lhs) / rhs;
}

template<size_t N>
inline constexpr auto Vector<N>::GetZero() -> Vector
{
	return Vector();
}