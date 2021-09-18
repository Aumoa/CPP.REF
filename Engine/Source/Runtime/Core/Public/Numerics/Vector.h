// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <initializer_list>
#include <string>
#include <span>
#include <array>
#include <type_traits>
#include "PrimitiveTypes.h"
#include "Mathematics/MathEx.h"
#include "Misc/StringUtils.h"
#include "Container/BitSet.h"
#include "Concepts/CoreConcepts.h"

template<size_t N>
struct VectorScalarsImpl
{
	static constexpr size_t Count = N;
	
	union
	{
		float Scalars[Count];
		struct
		{
			float X;
			float Y;
			float Z;
			float W;
		};
	};

	constexpr VectorScalarsImpl(float scalar = 0)
	{
		for (size_t i = 0; i < Count; ++i)
		{
			Scalars[i] = scalar;
		}
	}

	constexpr VectorScalarsImpl(const VectorScalarsImpl& impl)
	{
		for (size_t i = 0; i < Count; ++i)
		{
			Scalars[i] = impl.Scalars[i];
		}
	}

	template<class... TArgs> requires (sizeof...(TArgs) <= Count && sizeof...(TArgs) >= 2)
	constexpr VectorScalarsImpl(const TArgs&... args) : Scalars{ args... }
	{
	}
};

template<>
struct VectorScalarsImpl<1>
{
	static constexpr size_t Count = 1;

	union
	{
		float Scalars[Count];
		struct
		{
			float X;
		};
	};

	constexpr VectorScalarsImpl(float scalar = 0)
	{
		for (size_t i = 0; i < Count; ++i)
		{
			Scalars[i] = scalar;
		}
	}

	constexpr VectorScalarsImpl(const VectorScalarsImpl& impl)
	{
		for (size_t i = 0; i < Count; ++i)
		{
			Scalars[i] = impl.Scalars[i];
		}
	}
};

template<>
struct VectorScalarsImpl<2>
{
	static constexpr size_t Count = 2;

	union
	{
		float Scalars[Count];
		struct
		{
			float X;
			float Y;
		};
	};

	constexpr VectorScalarsImpl(float scalar = 0)
	{
		for (size_t i = 0; i < Count; ++i)
		{
			Scalars[i] = scalar;
		}
	}

	constexpr VectorScalarsImpl(const VectorScalarsImpl& impl)
	{
		for (size_t i = 0; i < Count; ++i)
		{
			Scalars[i] = impl.Scalars[i];
		}
	}

	template<class... TArgs> requires (sizeof...(TArgs) <= Count && sizeof...(TArgs) >= 2)
	constexpr VectorScalarsImpl(const TArgs&... args) : Scalars{ args... }
	{
	}
};

template<>
struct VectorScalarsImpl<3>
{
	static constexpr size_t Count = 3;

	union
	{
		float Scalars[Count];
		struct
		{
			float X;
			float Y;
			float Z;
		};
	};

	constexpr VectorScalarsImpl(float scalar = 0)
	{
		for (size_t i = 0; i < Count; ++i)
		{
			Scalars[i] = scalar;
		}
	}

	constexpr VectorScalarsImpl(const VectorScalarsImpl& impl)
	{
		for (size_t i = 0; i < Count; ++i)
		{
			Scalars[i] = impl.Scalars[i];
		}
	}

	template<class... TArgs> requires (sizeof...(TArgs) <= Count && sizeof...(TArgs) >= 2)
	constexpr VectorScalarsImpl(const TArgs&... args) : Scalars{ args... }
	{
	}
};

template<class TVectorImpl, size_t N>
struct VectorImpl : public VectorScalarsImpl<N>
{
	using ScalarsImpl = VectorScalarsImpl<N>;

	constexpr VectorImpl() : ScalarsImpl()
	{
	}

	constexpr VectorImpl(float scalar) : ScalarsImpl(scalar)
	{
	}

	template<class T, class... TArgs> requires
		(N >= 2 && (sizeof...(TArgs) + 1 >= 2) && (sizeof...(TArgs) + 1 <= ScalarsImpl::Count))
	constexpr VectorImpl(const T& first, const TArgs&... args)
		: VectorImpl(GetValueTuple<0>(first, args...), std::make_index_sequence<N>{})
	{
	}

	template<class TOtherImpl, size_t _ON>
	constexpr VectorImpl(const VectorImpl<TOtherImpl, _ON>& impl)
		: VectorImpl(GetValueTuple<0>(impl), std::make_index_sequence<_ON>{})
	{
	}

	template<class TOtherImpl>
	constexpr VectorImpl(const VectorImpl<TOtherImpl, N>& impl) : ScalarsImpl(impl)
	{
	}

private:
	template<class... TArgs, size_t... _Seq>
	constexpr VectorImpl(const std::tuple<TArgs...>& valueTuple, const std::index_sequence<_Seq...>&)
		: ScalarsImpl(std::get<_Seq>(valueTuple)...)
	{
	}

	template<size_t _InternalIdx, class T>
	static constexpr float GetValueTupleImpl(const T& value)
	{
		if constexpr (std::convertible_to<std::remove_const_t<std::remove_reference_t<T>>, float>)
		{
			return (float)value;
		}
		else
		{
			return (float)value.Scalars[_InternalIdx];
		}
	}

	template<size_t _InternalIdx, class T> requires std::convertible_to<std::remove_const_t<std::remove_reference_t<T>>, float>
	static consteval size_t MoveInternalIndexImpl()
	{
		return 0;
	}

	template<size_t _InternalIdx, class T, decltype(T::Count)* = nullptr>
	static consteval size_t MoveInternalIndexImpl()
	{
		constexpr size_t _Nx = _InternalIdx + 1;
		return (_Nx == T::Count) ? 0 : _Nx;
	}

	template<size_t _InternalIdx, class T, class... TArgs>
	static constexpr auto GetValueTuple(const T& value, const TArgs&... args)
	{
		constexpr size_t _Nx = MoveInternalIndexImpl<_InternalIdx, T>();
		std::tuple tup = std::make_tuple(GetValueTupleImpl<_InternalIdx, T>(value));

		if constexpr (_Nx != 0)
		{
			return std::tuple_cat(tup, GetValueTuple<_Nx>(value, args...));
		}
		else if constexpr (sizeof...(TArgs) != 0)
		{
			return std::tuple_cat(tup, GetValueTuple<_Nx>(args...));
		}
		else
		{
			return tup;
		}
	}

public:
	struct SelectControl
	{
		BitSet<N> Bits;

		template<class... TArgs> requires (sizeof...(TArgs) <= ScalarsImpl::Count)
		inline constexpr SelectControl(TArgs&&... args) : Bits(std::forward<TArgs>(args)...)
		{
		}

		static inline constexpr SelectControl Less(const TVectorImpl& lhs, const TVectorImpl& rhs)
		{
			return Less_impl(lhs, rhs, std::make_index_sequence<N>{});
		}

		static inline constexpr SelectControl Greater(const TVectorImpl& lhs, const TVectorImpl& rhs)
		{
			return Greater_impl(lhs, rhs, std::make_index_sequence<N>{});
		}

	private:
		template<size_t... idx>
		static inline constexpr SelectControl Less_impl(const TVectorImpl& lhs, const TVectorImpl& rhs, std::index_sequence<idx...>&&)
		{
			return SelectControl((lhs[idx] < rhs[idx])...);
		}

		template<size_t... idx>
		static inline constexpr SelectControl Greater_impl(const TVectorImpl& lhs, const TVectorImpl& rhs, std::index_sequence<idx...>&&)
		{
			return SelectControl((lhs[idx] > rhs[idx])...);
		}
	};

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
			v += ScalarsImpl::Scalars[i] * ScalarsImpl::Scalars[i];
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
	inline constexpr bool NearlyEquals(const TVectorImpl& rhs, float epsilon = MathEx::SmallNumber) const
	{
		for (size_t i = 0; i < N; ++i)
		{
			if (MathEx::Abs(ScalarsImpl::Scalars[i] - rhs.Scalars[i]) > epsilon)
			{
				return false;
			}
		}
		return true;
	}

	inline constexpr const float& operator [](size_t idx) const
	{
		return ScalarsImpl::Scalars[idx];
	}

	inline float& operator [](size_t idx)
	{
		return ScalarsImpl::Scalars[idx];
	}

	inline constexpr bool operator ==(const TVectorImpl& rhs) const
	{
		for (size_t i = 0; i < N; ++i)
		{
			if (ScalarsImpl::Scalars[i] != rhs.Scalars[i])
			{
				return false;
			}
		}
		return true;
	}

	inline constexpr bool operator !=(const TVectorImpl& rhs) const
	{
		for (size_t i = 0; i < N; ++i)
		{
			if (ScalarsImpl::Scalars[i] == rhs.Scalars[i])
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
	static inline constexpr float GetDistanceSq(const TVectorImpl& lhs, const TVectorImpl& rhs)
	{
		return (rhs - lhs).GetLengthSq();
	}

	/// <summary>
	/// Get distance between two vectors.
	/// </summary>
	/// <param name="lhs"> The first vector. </param>
	/// <param name="rhs"> The second vector. </param>
	/// <returns> The distance between two vectors. </returns>
	static inline float GetDistance(const TVectorImpl& lhs, const TVectorImpl& rhs)
	{
		return (rhs - lhs).GetLength();
	}

	/// <summary>
	/// Get normalized vector.
	/// </summary>
	/// <returns> The normalized vector. </returns>
	inline TVectorImpl GetNormal() const
	{
		float invSqrt = MathEx::InvSqrt(GetLengthSq());
		TVectorImpl result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Scalars[i] = ScalarsImpl::Scalars[i] * invSqrt;
		}
		return result;
	}

	inline constexpr TVectorImpl operator -() const
	{
		TVectorImpl result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Scalars[i] = -ScalarsImpl::Scalars[i];
		}
		return result;
	}

	inline constexpr TVectorImpl operator +(const TVectorImpl& rhs) const
	{
		TVectorImpl result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Scalars[i] = ScalarsImpl::Scalars[i] + rhs.Scalars[i];
		}
		return result;
	}

	inline constexpr TVectorImpl operator -(const TVectorImpl& rhs) const
	{
		TVectorImpl result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Scalars[i] = ScalarsImpl::Scalars[i] - rhs.Scalars[i];
		}
		return result;
	}

	inline constexpr TVectorImpl operator *(const TVectorImpl& rhs) const
	{
		TVectorImpl result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Scalars[i] = ScalarsImpl::Scalars[i] * rhs.Scalars[i];
		}
		return result;
	}

	inline constexpr TVectorImpl operator /(const TVectorImpl& rhs) const
	{
		TVectorImpl result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Scalars[i] = ScalarsImpl::Scalars[i] / rhs.Scalars[i];
		}
		return result;
	}

	inline constexpr TVectorImpl operator *(float rhs) const
	{
		TVectorImpl result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Scalars[i] = ScalarsImpl::Scalars[i] * rhs;
		}
		return result;
	}

	inline constexpr TVectorImpl operator /(float rhs) const
	{
		TVectorImpl result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Scalars[i] = ScalarsImpl::Scalars[i] / rhs;
		}
		return result;
	}

	inline TVectorImpl& operator =(const TVectorImpl& rhs)
	{
		for (size_t i = 0; i < N; ++i)
		{
			ScalarsImpl::Scalars[i] = rhs.Scalars[i];
		}
		return static_cast<TVectorImpl&>(*this);
	}

	inline TVectorImpl& operator +=(const TVectorImpl& rhs)
	{
		for (size_t i = 0; i < N; ++i)
		{
			ScalarsImpl::Scalars[i] += rhs.Scalars[i];
		}
		return static_cast<TVectorImpl&>(*this);
	}

	inline TVectorImpl& operator -=(const TVectorImpl& rhs)
	{
		for (size_t i = 0; i < N; ++i)
		{
			ScalarsImpl::Scalars[i] -= rhs.Scalars[i];
		}
		return static_cast<TVectorImpl&>(*this);
	}

	inline TVectorImpl& operator *=(const TVectorImpl& rhs)
	{
		for (size_t i = 0; i < N; ++i)
		{
			ScalarsImpl::Scalars[i] *= rhs.Scalars[i];
		}
		return static_cast<TVectorImpl&>(*this);
	}

	inline TVectorImpl& operator /=(const TVectorImpl& rhs)
	{
		for (size_t i = 0; i < N; ++i)
		{
			ScalarsImpl::Scalars[i] /= rhs.Scalars[i];
		}
		return static_cast<TVectorImpl&>(*this);
	}

	inline TVectorImpl& operator *=(float rhs)
	{
		for (size_t i = 0; i < N; ++i)
		{
			ScalarsImpl::Scalars[i] *= rhs;
		}
		return static_cast<TVectorImpl&>(*this);
	}

	inline TVectorImpl& operator /=(float rhs)
	{
		for (size_t i = 0; i < N; ++i)
		{
			ScalarsImpl::Scalars[i] /= rhs;
		}
		return static_cast<TVectorImpl&>(*this);
	}

	/// <summary>
	/// Get vector composed of each maximum components.
	/// </summary>
	/// <param name="lhs"> The first vector. </param>
	/// <param name="rhs"> The second vector.</param>
	/// <returns> The result vector. </returns>
	static inline constexpr TVectorImpl Max(const TVectorImpl& lhs, const TVectorImpl& rhs)
	{
		TVectorImpl result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Scalars[i] = MathEx::Max(lhs.Scalars[i], rhs.Scalars[i]);
		}
		return result;
	}

	/// <summary>
	/// Get vector composed of each minimum components.
	/// </summary>
	/// <param name="lhs"> The first vector. </param>
	/// <param name="rhs"> The second vector.</param>
	/// <returns> The result vector. </returns>
	static inline constexpr TVectorImpl Min(const TVectorImpl& lhs, const TVectorImpl& rhs)
	{
		TVectorImpl result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Scalars[i] = MathEx::Min(lhs.Scalars[i], rhs.Scalars[i]);
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
			composed[i] = std::format(placeholder, ScalarsImpl::Scalars[i]);
		}

		return std::format(L"{{{}}}", StringUtils::Join(L", ", std::span<std::wstring const>(composed)));
	}

	inline constexpr TVectorImpl Round() const
	{
		TVectorImpl V;
		for (size_t i = 0; i < N; ++i)
		{
			V.Scalars[i] = MathEx::Round(ScalarsImpl::Scalars[i]);
		}
		return V;
	}

	inline static constexpr TVectorImpl Select(const TVectorImpl& lhs, const TVectorImpl& rhs, const SelectControl& select)
	{
		TVectorImpl V;
		for (size_t i = 0; i < N; ++i)
		{
			if (select.Bits.GetBit(i))
			{
				V[i] = rhs[i];
			}
			else
			{
				V[i] = lhs[i];
			}
		}
		return V;
	}
};

template<template<size_t> class TVectorImpl, size_t N, std::enable_if_t<std::is_base_of_v<VectorImpl<TVectorImpl<N>, N>, TVectorImpl<N>>, void*> = nullptr>
inline constexpr TVectorImpl<N> operator *(float lhs, const TVectorImpl<N>& rhs)
{
	return TVectorImpl(VectorImpl<TVectorImpl<N>, N>(lhs) * rhs);
}

template<template<size_t> class TVectorImpl, size_t N, std::enable_if_t<std::is_base_of_v<VectorImpl<TVectorImpl<N>, N>, TVectorImpl<N>>, void*> = nullptr>
inline constexpr TVectorImpl<N> operator /(float lhs, const TVectorImpl<N>& rhs)
{
	return TVectorImpl(VectorImpl<TVectorImpl<N>, N>(lhs) / rhs);
}

/// <summary>
/// Represents a vector with specified count floating values.
/// </summary>
template<size_t N>
struct Vector : public VectorImpl<Vector<N>, N>
{
	using Impl = VectorImpl<Vector<N>, N>;

	/// <summary>
	/// Initialize new <see cref="Vector"/> instance.
	/// </summary>
	template<class... TArgs> requires Constructible<Impl, TArgs...>
	inline constexpr Vector(const TArgs&... args) : Impl(args...)
	{
	}

	/// <summary>
	/// Calculate cross product.
	/// </summary>
	/// <param name="lhs"> The first value. </param>
	/// <param name="rhs"> The second value. </param>
	/// <returns> The vector of result of cross product. </returns>
	template<class...> requires (N == 2)
	inline static constexpr float CrossProduct(const Vector& lhs, const Vector& rhs)
	{
		return lhs.X * rhs.Y - lhs.Y * rhs.X;
	}

	/// <summary>
	/// Calculate cross product.
	/// </summary>
	/// <param name="lhs"> The first value. </param>
	/// <param name="rhs"> The second value. </param>
	/// <returns> The vector of result of cross product. </returns>
	template<class...> requires (N == 3)
	inline static constexpr Vector CrossProduct(const Vector& lhs, const Vector& rhs)
	{
		return
		{
			lhs.Y * rhs.Z - lhs.Z * rhs.Y,
			lhs.Z * rhs.X - lhs.X * rhs.Z,
			lhs.X * rhs.Y - lhs.Y * rhs.X
		};
	}

	template<class...> requires (N == 3)
	inline constexpr Vector operator ^(const Vector& rhs) const
	{
		return CrossProduct(*this, rhs);
	}

	/// <summary>
	/// Swizzling vector that compose components specified indexes.
	/// </summary>
	/// <returns> Composed vector. </returns>
	template<size_t... Indexes>
	inline constexpr Vector<sizeof...(Indexes)> Swiz() const
	{
		return Vector<sizeof...(Indexes)>{ Impl::Scalars[Indexes]... };
	}

	/// <summary>
	/// Get vector that removed single scalar value.
	/// </summary>
	template<class...> requires (N > 1)
	inline constexpr Vector<N - 1> Minor(size_t idx) const
	{
		Vector<N - 1> results;
		for (size_t i = 0; i < N; ++i)
		{
			if (i < idx)
			{
				results.Scalars[i] = Impl::Scalars[i];
			}
			else if (i > idx)
			{
				results.Scalars[i - 1] = Impl::Scalars[i];
			}
		}
		return results;
	}

	inline static constexpr Vector GetZero() { return Vector(); }
	inline static constexpr Vector GetOneVector() { return Vector(1.0f); }
};

using Vector2 = Vector<2>;
using Vector3 = Vector<3>;
using Vector4 = Vector<4>;