// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <initializer_list>
#include <string>
#include <span>
#include <array>
#include <type_traits>
#include "PrimitiveTypes.h"
#include "VectorScalarsImpl.h"
#include "Mathematics/MathEx.h"
#include "Misc/StringUtils.h"
#include "Container/BitSet.h"
#include "Concepts/CoreConcepts.h"

template<class T, class TVector, size_t N>
struct VectorImpl : public VectorScalarsImpl<T, N>
{
	using ScalarsImpl = VectorScalarsImpl<T, N>;

	constexpr VectorImpl() : ScalarsImpl()
	{
	}

	constexpr VectorImpl(const T& scalar) : ScalarsImpl(scalar)
	{
	}

	template<class U, class... TArgs> requires
		(N >= 2 && (sizeof...(TArgs) + 1 >= 2) && (sizeof...(TArgs) + 1 <= ScalarsImpl::Count))
	constexpr VectorImpl(const U& first, const TArgs&... args)
		: VectorImpl(GetValueTuple<0>(first, args...), std::make_index_sequence<N>{})
	{
	}

	template<class TOtherImpl, size_t _ON>
	constexpr VectorImpl(const VectorImpl<T, TOtherImpl, _ON>& impl)
		: VectorImpl(GetValueTuple<0>(impl), std::make_index_sequence<_ON>{})
	{
	}

	template<class TOtherImpl>
	constexpr VectorImpl(const VectorImpl<T, TOtherImpl, N>& impl) : ScalarsImpl(impl)
	{
	}

private:
	template<class... TArgs, size_t... _Seq>
	constexpr VectorImpl(const std::tuple<TArgs...>& valueTuple, const std::index_sequence<_Seq...>&)
		: ScalarsImpl(std::get<_Seq>(valueTuple)...)
	{
	}

	template<size_t _InternalIdx, class U>
	static constexpr T GetValueTupleImpl(const U& value)
	{
		if constexpr (std::convertible_to<std::remove_const_t<std::remove_reference_t<U>>, T>)
		{
			return (T)value;
		}
		else
		{
			return (T)value.Scalars[_InternalIdx];
		}
	}

	template<size_t _InternalIdx, class U> requires std::convertible_to<std::remove_const_t<std::remove_reference_t<U>>, T>
	static consteval size_t MoveInternalIndexImpl()
	{
		return 0;
	}

	template<size_t _InternalIdx, class U, decltype(U::Count)* = nullptr>
	static consteval size_t MoveInternalIndexImpl()
	{
		constexpr size_t _Nx = _InternalIdx + 1;
		return (_Nx == U::Count) ? 0 : _Nx;
	}

	template<size_t _InternalIdx, class U, class... TArgs>
	static constexpr auto GetValueTuple(const U& value, const TArgs&... args)
	{
		constexpr size_t _Nx = MoveInternalIndexImpl<_InternalIdx, U>();
		std::tuple tup = std::make_tuple(GetValueTupleImpl<_InternalIdx, U>(value));

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

	template<size_t... _Idx>
	inline constexpr T SizeImpl(std::index_sequence<_Idx...>&&) const
	{
		return (ScalarsImpl::Scalars[_Idx] * ...);
	}

public:
	struct SelectControl
	{
		BitSet<N> Bits;

		template<class... TArgs> requires (sizeof...(TArgs) <= ScalarsImpl::Count)
		inline constexpr SelectControl(TArgs&&... args) : Bits(std::forward<TArgs>(args)...)
		{
		}

		static inline constexpr SelectControl Less(const TVector& lhs, const TVector& rhs)
		{
			return Less_impl(lhs, rhs, std::make_index_sequence<N>{});
		}

		static inline constexpr SelectControl Greater(const TVector& lhs, const TVector& rhs)
		{
			return Greater_impl(lhs, rhs, std::make_index_sequence<N>{});
		}

	private:
		template<size_t... idx>
		static inline constexpr SelectControl Less_impl(const TVector& lhs, const TVector& rhs, std::index_sequence<idx...>&&)
		{
			return SelectControl((lhs[idx] < rhs[idx])...);
		}

		template<size_t... idx>
		static inline constexpr SelectControl Greater_impl(const TVector& lhs, const TVector& rhs, std::index_sequence<idx...>&&)
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
	/// Get value of multiply all components.
	/// </summary>
	inline constexpr T Size() const
	{
		return SizeImpl(std::make_index_sequence<N>{});
	}

	/// <summary>
	/// Get squared length of this vector.
	/// </summary>
	/// <returns> The squared length. </returns>
	inline constexpr T GetLengthSq() const
	{
		T v = 0;
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
	inline float GetLength() const requires std::is_floating_point_v<T>
	{
		return MathEx::Sqrt(GetLengthSq());
	}

	/// <summary>
	/// Compare nearly equals to target vector with epsilon value.
	/// </summary>
	/// <param name="rhs"> The target vector. </param>
	/// <param name="epsilon"> The epsilon value. If different of two components is lower than this values, is nearly equals. </param>
	/// <returns> Indicate two vectors is nearly equals. </returns>
	inline constexpr bool NearlyEquals(const TVector& rhs, float epsilon = MathEx::SmallNumber) const requires std::is_floating_point_v<T>
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

	inline constexpr const T& operator [](size_t idx) const
	{
		return ScalarsImpl::Scalars[idx];
	}

	inline T& operator [](size_t idx)
	{
		return ScalarsImpl::Scalars[idx];
	}

	inline constexpr bool operator ==(const TVector& rhs) const
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

	inline constexpr bool operator !=(const TVector& rhs) const
	{
		for (size_t i = 0; i < N; ++i)
		{
			if (ScalarsImpl::Scalars[i] != rhs.Scalars[i])
			{
				return true;
			}
		}
		return false;
	}

	/// <summary>
	/// Get squared distance between two vectors.
	/// </summary>
	/// <param name="lhs"> The first vector. </param>
	/// <param name="rhs"> The second vector. </param>
	/// <returns> The squared distance between two vectors. </returns>
	static inline constexpr float GetDistanceSq(const TVector& lhs, const TVector& rhs) requires std::is_floating_point_v<T>
	{
		return (rhs - lhs).GetLengthSq();
	}

	/// <summary>
	/// Get distance between two vectors.
	/// </summary>
	/// <param name="lhs"> The first vector. </param>
	/// <param name="rhs"> The second vector. </param>
	/// <returns> The distance between two vectors. </returns>
	static inline float GetDistance(const TVector& lhs, const TVector& rhs) requires std::is_floating_point_v<T>
	{
		return (rhs - lhs).GetLength();
	}

	/// <summary>
	/// Get normalized vector.
	/// </summary>
	/// <returns> The normalized vector. </returns>
	inline TVector GetNormal() const requires std::is_floating_point_v<T>
	{
		float invSqrt = MathEx::InvSqrt(GetLengthSq());
		TVector result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Scalars[i] = ScalarsImpl::Scalars[i] * invSqrt;
		}
		return result;
	}

	inline constexpr TVector operator -() const
	{
		TVector result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Scalars[i] = -ScalarsImpl::Scalars[i];
		}
		return result;
	}

#define DEFINE_BINARY_OPERATOR(Op)												\
	inline constexpr TVector operator Op(const TVector& rhs) const				\
	{																			\
		TVector result;															\
		for (size_t i = 0; i < N; ++i)											\
		{																		\
			result.Scalars[i] = ScalarsImpl::Scalars[i] Op rhs.Scalars[i];		\
		}																		\
		return result;															\
	}																			\
																				\
	inline constexpr TVector operator Op(const T& rhs) const					\
	{																			\
		TVector result;															\
		for (size_t i = 0; i < N; ++i)											\
		{																		\
			result.Scalars[i] = ScalarsImpl::Scalars[i] Op rhs;					\
		}																		\
		return result;															\
	}

	DEFINE_BINARY_OPERATOR(+);
	DEFINE_BINARY_OPERATOR(-);
	DEFINE_BINARY_OPERATOR(*);
	DEFINE_BINARY_OPERATOR(/);

#undef DEFINE_BINARY_OPERATOR

	inline constexpr TVector operator %(const TVector& rhs) const requires std::is_floating_point_v<T>
	{
		TVector result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Scalars[i] = MathEx::Mod(ScalarsImpl::Scalars[i], rhs.Scalars[i]);
		}
		return result;
	}
	
	inline constexpr TVector operator %(const TVector& rhs) const requires std::is_integral_v<T>
	{
		TVector result;
		for (size_t i = 0; i < N; ++i)
		{
			result.Scalars[i] = ScalarsImpl::Scalars[i] % rhs.Scalars[i];
		}
		return result;
	}

#define DEFINE_ASSIGNMENT_OPERATOR(Op)							\
	inline TVector& operator Op(const TVector& rhs)				\
	{															\
		for (size_t i = 0; i < N; ++i)							\
		{														\
			ScalarsImpl::Scalars[i] Op rhs.Scalars[i];			\
		}														\
		return static_cast<TVector&>(*this);					\
	}															\
																\
	inline TVector& operator Op(const T& rhs)					\
	{															\
		for (size_t i = 0; i < N; ++i)							\
		{														\
			ScalarsImpl::Scalars[i] Op rhs;						\
		}														\
		return static_cast<TVector&>(*this);					\
	}

	DEFINE_ASSIGNMENT_OPERATOR(= );
	DEFINE_ASSIGNMENT_OPERATOR(+=);
	DEFINE_ASSIGNMENT_OPERATOR(-=);
	DEFINE_ASSIGNMENT_OPERATOR(*=);
	DEFINE_ASSIGNMENT_OPERATOR(/=);

#undef DEFINE_ASSIGNMENT_OPERATOR

	inline TVector& operator %=(const TVector& rhs) requires std::is_floating_point_v<T>
	{
		for (size_t i = 0; i < N; ++i)
		{
			ScalarsImpl::Scalars[i] = MathEx::Mod(ScalarsImpl::Scalars[i], rhs.Scalars[i]);
		}
		return static_cast<TVector&>(*this);
	}

	inline TVector& operator %=(const TVector& rhs) requires std::is_integral_v<T>
	{
		for (size_t i = 0; i < N; ++i)
		{
			ScalarsImpl::Scalars[i] %= rhs.Scalars[i];
		}
		return static_cast<TVector&>(*this);
	}

	/// <summary>
	/// Get vector composed of each maximum components.
	/// </summary>
	/// <param name="lhs"> The first vector. </param>
	/// <param name="rhs"> The second vector.</param>
	/// <returns> The result vector. </returns>
	static inline constexpr TVector Max(const TVector& lhs, const TVector& rhs)
	{
		TVector result;
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
	static inline constexpr TVector Min(const TVector& lhs, const TVector& rhs)
	{
		TVector result;
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

	inline constexpr TVector Round() const requires std::is_floating_point_v<T>
	{
		TVector V;
		for (size_t i = 0; i < N; ++i)
		{
			V.Scalars[i] = MathEx::Round(ScalarsImpl::Scalars[i]);
		}
		return V;
	}

	inline static constexpr TVector Select(const TVector& lhs, const TVector& rhs, const SelectControl& select)
	{
		TVector V;
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

#define DEFINE_GLOBAL_OPERATOR(Op) \
template<class T, template<class, size_t> class TVector, size_t N, std::enable_if_t<std::is_base_of_v<VectorImpl<T, TVector<T, N>, N>, TVector<T, N>>, void*> = nullptr>\
inline constexpr TVector<T, N> operator Op(const T& lhs, const TVector<T, N>& rhs)\
{\
	return TVector(VectorImpl<T, TVector<T, N>, N>(lhs) Op rhs);\
}

DEFINE_GLOBAL_OPERATOR(+);
DEFINE_GLOBAL_OPERATOR(-);
DEFINE_GLOBAL_OPERATOR(*);
DEFINE_GLOBAL_OPERATOR(/);
DEFINE_GLOBAL_OPERATOR(%);

#undef DEFINE_GLOBAL_OPERATOR

/// <summary>
/// Represents a vector with specific count of template values.
/// </summary>
template<class T, size_t N>
struct Vector : public VectorImpl<T, Vector<T, N>, N>
{
	using Impl = VectorImpl<T, Vector<T, N>, N>;

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
	template<class...> requires (N == 2) && std::is_floating_point_v<T>
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
	template<class...> requires (N == 3) && std::is_floating_point_v<T>
	inline static constexpr Vector CrossProduct(const Vector& lhs, const Vector& rhs)
	{
		return
		{
			lhs.Y * rhs.Z - lhs.Z * rhs.Y,
			lhs.Z * rhs.X - lhs.X * rhs.Z,
			lhs.X * rhs.Y - lhs.Y * rhs.X
		};
	}

	template<class...> requires (N == 3) && std::is_floating_point_v<T>
	inline constexpr Vector operator ^(const Vector& rhs) const
	{
		return CrossProduct(*this, rhs);
	}

	/// <summary>
	/// Swizzling vector that compose components specified indexes.
	/// </summary>
	/// <returns> Composed vector. </returns>
	template<size_t... Indexes>
	inline constexpr Vector<T, sizeof...(Indexes)> Swiz() const
	{
		return Vector<T, sizeof...(Indexes)>{ Impl::Scalars[Indexes]... };
	}

	/// <summary>
	/// Get vector that removed single scalar value.
	/// </summary>
	template<class...> requires (N > 1)
	inline constexpr Vector<T, N - 1> Minor(size_t idx) const
	{
		Vector<T, N - 1> results;
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

	/// <summary>
	/// Casts to specific type of vector element.
	/// </summary>
	/// <typeparam name="U"> Type of your desired. </typeparam>
	template<class U>
	inline constexpr Vector<U, N> Cast() const
	{
		return CastImpl<U>(std::make_index_sequence<N>{});
	}

	inline static constexpr Vector ZeroVector() { return Vector(); }
	inline static constexpr Vector OneVector() { return Vector((T)1); }

private:
	template<class U, size_t... _Idx>
	inline constexpr Vector<U, N> CastImpl(std::index_sequence<_Idx...>&&) const
	{
		return Vector<U, N>(static_cast<U>(Impl::Scalars[_Idx])...);
	}
};

namespace std
{
	template<class T, size_t N>
	struct tuple_size<Vector<T, N>> : public integral_constant<size_t, N>
	{
	};

	template<size_t Idx,class T, size_t N>
	struct tuple_element<Idx, Vector<T, N>>
	{
		using type = T;
	};

	template<size_t Idx, class T, size_t N>
	inline tuple_element_t<Idx, Vector<T, N>>& get(Vector<T, N>& value)
	{
		return value.Scalars[Idx];
	}

	template<size_t Idx, class T, size_t N>
	inline const tuple_element_t<Idx, Vector<T, N>>& get(const Vector<T, N>& value)
	{
		return value.Scalars[Idx];
	}
}

using Vector2 = Vector<float, 2>;
using Vector3 = Vector<float, 3>;
using Vector4 = Vector<float, 4>;

using Vector2N = Vector<int32, 2>;
using Vector3N = Vector<int32, 3>;
using Vector4N = Vector<int32, 4>;