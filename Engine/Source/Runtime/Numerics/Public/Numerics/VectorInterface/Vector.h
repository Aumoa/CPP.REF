// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/NumericConcepts.h"
#include "Numerics/VectorInterface/VectorScalarsImpl.h"

template<class T = void, size_t N = 0>
struct Vector : public VectorScalarsImpl<T, N>
{
	using Super = VectorScalarsImpl<T, N>;

private:
	struct Private
	{
	};

public:
	template<class... TArgs>
	constexpr Vector(TArgs&&... Args)
		requires (sizeof...(TArgs) > 1) &&
		std::constructible_from<Super, TArgs...>
		: Super(std::forward<TArgs>(Args)...)
	{
	}

	template<class... TArgs>
	constexpr Vector(TArgs&&... Args) requires
		(sizeof...(TArgs) > 0) &&
		(!std::constructible_from<Super, TArgs...>)
		: Vector(Private{}, std::tuple_cat(MakeScalarsImpl(std::forward<TArgs>(Args))...))
	{
	}

	constexpr Vector(const Vector& V) : Super((const Super&)V)
	{
	}

	template<TIsVector<T, N> IVector>
	constexpr Vector(const IVector& V)
		: Vector(Private{}, MakeScalarsImpl(V), std::make_index_sequence<IVector::Size()>{})
	{
	}

	constexpr Vector& operator =(const Vector& V)
	{
		for (size_t i = 0; i < Size(); ++i)
		{
			Super::Scalars[i] = V[i];
		}

		return *this;
	}

private:
	template<class... TArgs>
	constexpr Vector(Private&&, std::tuple<TArgs...>&& ArgsTup)
		: Vector(Private{}, std::move(ArgsTup), std::make_index_sequence<sizeof...(TArgs)>{})
	{
	}

	template<class... TArgs, size_t... Indexes>
	constexpr Vector(Private&&, std::tuple<TArgs...>&& ArgsTup, std::index_sequence<Indexes...>&&)
		: Super(std::get<Indexes>(ArgsTup)...)
	{
	}

	template<TIsVectorTyped<T> IVector>
	constexpr auto MakeScalarsImpl(const IVector& Forward)
	{
		return MakeScalarsImpl(Forward, std::make_index_sequence<IVector::Size()>{});
	}

	template<TIsVectorTyped<T> IVector, size_t... Indexes>
	constexpr auto MakeScalarsImpl(const IVector& Forward, std::index_sequence<Indexes...>&&)
	{
		return std::make_tuple(Forward[Indexes]...);
	}

	template<class TForward>
	constexpr auto MakeScalarsImpl(const TForward& Forward) requires requires { T(std::declval<TForward>()); }
	{
		return std::make_tuple(Forward);
	}

public:
	using Type = T;

	constexpr Vector(const T& S = {}) : Super(S)
	{
	}

	inline static constexpr size_t Size()
	{
		return N;
	}

	constexpr Vector operator -() const
	{
		Vector V;
		for (size_t i = 0; i < Size(); ++i)
		{
			V[i] = -Super::Scalars[i];
		}
		return V;
	}

	constexpr const T& operator [](size_t I) const
	{
		return Super::Scalars[I];
	}

	constexpr T& operator [](size_t I)
	{
		return Super::Scalars[I];
	}

	template<TIsVector<T, N> IVector>
	constexpr Vector& operator =(const IVector& V)
	{
		for (size_t i = 0; i < Size(); ++i)
		{
			Super::Scalars[i] = V[i];
		}

		return *this;
	}

public:
	String ToString() const;
	template<TIsVector<T, N> IVector>
	constexpr bool NearlyEquals(const IVector& V, const T& Epsilon) const;

	inline static constexpr Vector Zero()
	{
		return Vector();
	}
};

template<>
struct Vector<void, 0>
{
	template<TIsVectorBase IVector>
	static inline constexpr auto LengthSq(const IVector& V) -> typename IVector::Type
	{
		typename IVector::Type S = 0;
		for (size_t i = 0; i < V.Size(); ++i)
		{
			auto Load = V[i];
			S += Load * Load;
		}
		return S;
	}

	template<TIsVectorBase IVector>
	static inline float Length(const IVector& V)
	{
		return Math::Sqrt(LengthSq(V));
	}

	template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
	static inline constexpr float DistanceSq(const IVectorL& VL, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>
	{
		return LengthSq(VR - VL);
	}

	template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
	static inline constexpr float Distance(const IVectorL& VL, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>
	{
		return Length(VR - VL);
	}

	template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
	static inline constexpr bool NearlyEquals(const IVectorL& VL, const IVectorR& VR, const typename IVectorL::Type& Epsilon) requires TIsCompatibleVector<IVectorL, IVectorR>
	{
		for (size_t i = 0; i < VL.Size(); ++i)
		{
			if (Math::Abs(VL[i] - VR[i]) > Epsilon)
			{
				return false;
			}
		}

		return true;
	}

	template<TIsVectorBase IVector, TIsVectorBase IVectorResult = IVector>
	static inline IVectorResult Normalize(const IVector& V) requires TIsCompatibleVector<IVector, IVectorResult>
	{
		float InvSqrt = Math::InvSqrt(LengthSq(V));
		IVectorResult R;
		for (size_t i = 0; i < V.Size(); ++i)
		{
			R[i] = V[i] * InvSqrt;
		}
		return R;
	}

	template<TIsVectorBase IVectorL, TIsVectorBase IVectorR, TIsVectorBase IVectorResult = IVectorL>
	static inline constexpr bool Max(const IVectorL& VL, const IVectorR& VR) requires
		TIsCompatibleVector<IVectorL, IVectorR>&&
		TIsCompatibleVector<IVectorL, IVectorResult>
	{
		IVectorResult R;
		for (size_t i = 0; i < VL.Size(); ++i)
		{
			R[i] = Math::Max(VL[i], VR[i]);
		}
		return R;
	}

	template<TIsVectorBase IVectorL, TIsVectorBase IVectorR, TIsVectorBase IVectorResult = IVectorL>
	static inline constexpr bool Min(const IVectorL& VL, const IVectorR& VR) requires
		TIsCompatibleVector<IVectorL, IVectorR>&&
		TIsCompatibleVector<IVectorL, IVectorResult>
	{
		IVectorResult R;
		for (size_t i = 0; i < VL.Size(); ++i)
		{
			R[i] = Math::Min(VL[i], VR[i]);
		}
		return R;
	}

	template<TIsVectorBase IVectorL, TIsVectorBase IVectorResult = IVectorL>
	static inline constexpr bool Clamp(const IVectorL& VL, const typename IVectorL::Type& Minimum, const typename IVectorL::Type& Maximum) requires
		TIsCompatibleVector<IVectorL, IVectorResult>
	{
		IVectorResult R;
		for (size_t i = 0; i < VL.Size(); ++i)
		{
			R[i] = Math::Clamp(VL[i], Minimum, Maximum);
		}
		return R;
	}

	template<TIsVectorBase IVectorL, TIsVectorBase IVectorMin, TIsVectorBase IVectorMax, TIsVectorBase IVectorResult = IVectorL>
	static inline constexpr bool Clamp(const IVectorL& VL, const IVectorMin& Minimum, const IVectorMax& Maximum) requires
		TIsCompatibleVector<IVectorL, IVectorResult>&&
		TIsCompatibleVector<IVectorL, IVectorMin>&&
		TIsCompatibleVector<IVectorL, IVectorMax>
	{
		IVectorResult R;
		for (size_t i = 0; i < VL.Size(); ++i)
		{
			R[i] = Math::Clamp(VL[i], Minimum[i], Maximum[i]);
		}
		return R;
	}

	template<TIsVectorBase IVector>
	static inline String ToString(const IVector& V)
	{
		constexpr size_t N = IVector::Size();
		std::array<String, N> Composed;
		for (size_t i = 0; i < N; ++i)
		{
			Composed[i] = String::Format(TEXT("{}"), V[i]);
		}

		return String::Format(TEXT("{{{}}}"), String::Join(TEXT(", "), std::span<String const>(Composed)));
	}

	template<TIsVectorBase IVector, TIsVectorBase IVectorResult = IVector>
	static inline IVectorResult Round(const IVector& V) requires
		TIsCompatibleVector<IVector, IVectorResult>&&
		std::is_floating_point_v<typename IVector::Type>
	{
		constexpr size_t N = IVector::Size();

		IVectorResult R;
		for (size_t i = 0; i < N; ++i)
		{
			R[i] = Math::Round(V[i]);
		}
		return R;
	}

	template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
	static inline constexpr typename IVectorL::Type Cross(const IVectorL& VL, const IVectorR& VR) requires
		TIsCompatibleVector<IVectorL, IVectorR> &&
		(IVectorL::Size() == 2)
	{
		return VL[0] * VR[1] - VL[1] * VR[0];
	}

	template<TIsVectorBase IVectorL, TIsVectorBase IVectorR, TIsVectorBase IVectorResult = IVectorL>
	static inline constexpr IVectorResult Cross(const IVectorL& VL, const IVectorR& VR) requires
		TIsCompatibleVector<IVectorL, IVectorR>&&
		TIsCompatibleVector<IVectorL, IVectorResult> &&
		(IVectorL::Size() == 3)
	{
		IVectorResult R;
		R[0] = VL[1] * VR[2] - VL[2] * VR[1];
		R[1] = VL[2] * VR[0] - VL[0] * VR[2];
		R[2] = VL[0] * VR[1] - VL[1] * VR[0];
		return R;
	}

	template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
	static inline constexpr auto Dot(const IVectorL& VL, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>
	{
		using T = typename IVectorL::Type;
		T Value = (T)0;

		for (size_t i = 0; i < IVectorL::Size(); ++i)
		{
			Value += VL[i] * VR[i];
		}

		return Value;
	}

	template<TIsVectorBase IVectorResult, TIsVectorBase IVector>
	static inline constexpr IVectorResult Cast(const IVector& V)
	{
		IVectorResult R;
		size_t Size = Math::Min(V.Size(), IVectorResult::Size());
		for (size_t i = 0; i < Size; ++i)
		{
			R[i] = static_cast<typename IVectorResult::Type>(V[i]);
		}
		return R;
	}

	template<class PrimitiveType, TIsVectorBase IVector>
	static inline constexpr auto Cast(const IVector& V)->Vector<PrimitiveType, IVector::Size()>
		requires (!TIsVectorBase<PrimitiveType>)
	{
		Vector<PrimitiveType, IVector::Size()> R;
		for (size_t i = 0; i < V.Size(); ++i)
		{
			R[i] = static_cast<PrimitiveType>(V[i]);
		}
		return R;
	}

	template<size_t... Indexes, TIsVectorBase IVector>
	static inline constexpr auto Swizzling(const IVector& V)
	{
		std::array<size_t, sizeof...(Indexes)> IndexesArr = {Indexes...};
		Vector<typename IVector::Type, sizeof...(Indexes)> R;
		for (size_t i = 0; i < sizeof...(Indexes); ++i)
		{
			R[i] = V[IndexesArr[i]];
		}
		return R;
	}

	template<TIsVectorBase IVector>
	static inline constexpr auto Minor(const IVector& V, size_t Index) requires (IVector::Size() > 1)
	{
		using T = typename IVector::Type;
		constexpr size_t N = IVector::Size();
		Vector<T, N - 1> R;
		for (size_t i = 0; i < N; ++i)
		{
			if (i < Index)
			{
				R[i] = V[i];
			}
			else if (i > Index)
			{
				R[i - 1] = V[i];
			}
		}
		return R;
	}

	template<TIsVectorBase IVectorL, TIsVectorBase IVectorR, class _Fn>
	static inline constexpr auto Select(const IVectorL& VL, const IVectorR& VR, _Fn&& Op) requires TIsCompatibleVector<IVectorL, IVectorR>
	{
		Vector<typename IVectorL::Type, IVectorL::Size()> R;
		for (size_t i = 0; i < R.Size(); ++i)
		{
			auto VVL = VL[i], VVR = VR[i];
			R[i] = Op(VVL, VVR) ? VVL : VVR;
		}
		return R;
	}

	template<TIsVectorBase IVectorL, TIsVectorBase IVectorR, TIsVectorBase IConditionL, TIsVectorBase IConditionR, class _Fn>
	static inline constexpr auto Select(const IVectorL& VL, const IVectorR& VR, const IConditionL& CL, const IConditionR& CR, _Fn&& Op) requires
		TIsCompatibleVector<IVectorL, IVectorR>&&
		TIsCompatibleVector<IVectorL, IConditionL>&&
		TIsCompatibleVector<IVectorL, IConditionR>
	{
		Vector<typename IVectorL::Type, IVectorL::Size()> R;
		for (size_t i = 0; i < R.Size(); ++i)
		{
			R[i] = Op(CL[i], CR[i]) ? VL[i] : VR[i];
		}
		return R;
	}
};


template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
constexpr IVectorL operator +(const IVectorL& VL, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>
{
	IVectorL R;
	for (size_t i = 0; i < VL.Size(); ++i)
	{
		R[i] = VL[i] + VR[i];
	}
	return R;
}

template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
constexpr IVectorL operator -(const IVectorL& VL, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>
{
	IVectorL R;
	for (size_t i = 0; i < VL.Size(); ++i)
	{
		R[i] = VL[i] - VR[i];
	}
	return R;
}

template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
constexpr IVectorL operator *(const IVectorL& VL, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>
{
	IVectorL R;
	for (size_t i = 0; i < VL.Size(); ++i)
	{
		R[i] = VL[i] * VR[i];
	}
	return R;
}

template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
constexpr IVectorL operator /(const IVectorL& VL, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>
{
	IVectorL R;
	for (size_t i = 0; i < VL.Size(); ++i)
	{
		R[i] = VL[i] / VR[i];
	}
	return R;
}

template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
constexpr IVectorL operator %(const IVectorL& VL, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>
{
	IVectorL R;
	for (size_t i = 0; i < VL.Size(); ++i)
	{
		R[i] = Math::Mod(VL[i], VR[i]);
	}
	return R;
}
template<TIsVectorBase IVector>
constexpr IVector operator *(const IVector& VL, const typename IVector::Type& S)
{
	IVector R;
	for (size_t i = 0; i < VL.Size(); ++i)
	{
		R[i] = VL[i] * S;
	}
	return R;
}

template<TIsVectorBase IVector>
constexpr IVector operator /(const IVector& VL, const typename IVector::Type& S)
{
	IVector R;
	for (size_t i = 0; i < VL.Size(); ++i)
	{
		R[i] = VL[i] / S;
	}
	return R;
}

template<TIsVectorBase IVector>
constexpr IVector operator %(const IVector& VL, const typename IVector::Type& S)
{
	IVector R;
	for (size_t i = 0; i < VL.Size(); ++i)
	{
		R[i] = Math::Mod(VL[i], S);
	}
	return R;
}

template<TIsVectorBase IVector>
constexpr auto operator *(const typename IVector::Type& S, const IVector& V)
{
	return IVector(S) * V;
}

template<TIsVectorBase IVector>
constexpr auto operator /(const typename IVector::Type& S, const IVector& V)
{
	return IVector(S) / V;
}

template<TIsVectorBase IVector>
constexpr auto operator %(const typename IVector::Type& S, const IVector& V)
{
	return IVector(S) % V;
}

template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
constexpr auto operator ^(const IVectorL& VL, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>&& requires
{
	{ Vector<>::Cross(VL, VR) };
}
{
	return Vector<>::Cross(VL, VR);
}

template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
constexpr auto operator |(const IVectorL& VL, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>
{
	return Vector<>::Dot(VL, VR);
}


template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
decltype(auto) operator +=(IVectorL& V, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>&& std::assignable_from<IVectorL, IVectorL>
{
	return V = V + VR;
}

template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
decltype(auto) operator -=(IVectorL& V, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>&& std::assignable_from<IVectorL, IVectorL>
{
	return V = V - VR;
}

template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
decltype(auto) operator *=(IVectorL& V, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>&& std::assignable_from<IVectorL, IVectorL>
{
	return V = V * VR;
}

template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
decltype(auto) operator /=(IVectorL& V, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>&& std::assignable_from<IVectorL, IVectorL>
{
	return V = V / VR;
}

template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
decltype(auto) operator %=(IVectorL& V, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>&& std::assignable_from<IVectorL, IVectorL>
{
	return V = V % VR;
}

template<TIsVectorBase IVector>
decltype(auto) operator *=(IVector& V, const typename IVector::Type& S) requires std::assignable_from<IVector, IVector>
{
	return V = V * S;
}

template<TIsVectorBase IVector>
decltype(auto) operator /=(IVector& V, const typename IVector::Type& S) requires std::assignable_from<IVector, IVector>
{
	return V = V / S;
}

template<TIsVectorBase IVector>
decltype(auto) operator %=(IVector& V, const typename IVector::Type& S) requires std::assignable_from<IVector, IVector>
{
	return V = V % S;
}


template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
constexpr std::strong_ordering operator <=>(const IVectorL& VL, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>
{
	for (size_t i = 0; i < VL.Size(); ++i)
	{
		const auto R = VL[i] <=> VR[i];

		if (R < 0)
		{
			return std::strong_ordering::less;
		}
		else if (R > 0)
		{
			return std::strong_ordering::greater;
		}
	}

	return std::strong_ordering::equal;
}

template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
constexpr bool operator < (const IVectorL& VL, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>
{
	return operator <=>(VL, VR) < 0;
}

template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
constexpr bool operator <=(const IVectorL& VL, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>
{
	return operator <=>(VL, VR) <= 0;
}

template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
constexpr bool operator > (const IVectorL& VL, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>
{
	return operator <=>(VL, VR) > 0;
}

template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
constexpr bool operator >=(const IVectorL& VL, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>
{
	return operator <=>(VL, VR) >= 0;
}

template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
constexpr bool operator ==(const IVectorL& VL, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>
{
	return operator <=>(VL, VR) == 0;
}

template<TIsVectorBase IVectorL, TIsVectorBase IVectorR>
constexpr bool operator !=(const IVectorL& VL, const IVectorR& VR) requires TIsCompatibleVector<IVectorL, IVectorR>
{
	return operator <=>(VL, VR) != 0;
}


namespace std
{
	template<TIsVectorBase IVector>
	struct tuple_size<IVector> : public integral_constant<size_t, IVector::Size()>
	{
	};

	template<TIsVectorBase IVector>
	struct tuple_size<const IVector> : public tuple_size<IVector>
	{
	};

	template<size_t Idx, TIsVectorBase IVector>
	struct tuple_element<Idx, IVector>
	{
		using type = typename IVector::Type;
	};

	template<size_t Idx, TIsVectorBase IVector>
	struct tuple_element<Idx, const IVector> : public tuple_element<Idx, IVector>
	{
	};

	template<size_t Idx, TIsVectorBase IVector>
	inline tuple_element_t<Idx, IVector>& get(IVector& V)
	{
		return V[Idx];
	}

	template<size_t Idx, TIsVectorBase IVector>
	inline const tuple_element_t<Idx, IVector>& get(const IVector& V)
	{
		return V[Idx];
	}
}

using Vector2 = Vector<float, 2>;
using Vector3 = Vector<float, 3>;
using Vector4 = Vector<float, 4>;

using Vector2N = Vector<int32, 2>;
using Vector3N = Vector<int32, 3>;
using Vector4N = Vector<int32, 4>;

template<class T, size_t N>
String Vector<T, N>::ToString() const
{
	return Vector<>::ToString(*this);
}

template<class T, size_t N>
template<TIsVector<T, N> IVector>
constexpr bool Vector<T, N>::NearlyEquals(const IVector& V, const T& Epsilon) const
{
	return Vector<>::NearlyEquals(*this, V, Epsilon);
}