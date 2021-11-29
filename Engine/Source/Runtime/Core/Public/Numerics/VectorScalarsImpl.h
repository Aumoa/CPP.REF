// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"

template<class T, size_t N>
struct VectorScalarsImpl
{
	static constexpr size_t Count = N;
	
	union
	{
		T Scalars[Count];
		struct
		{
			T X;
			T Y;
			T Z;
			T W;
		};
	};

	constexpr VectorScalarsImpl(const T& scalar = 0)
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
	constexpr VectorScalarsImpl(const TArgs&... args) : Scalars{ static_cast<T>(args)... }
	{
	}
};

#define DECLARE_VECTOR_SCALARS_IMPL(ScalarsCount, Members)											\
template<class T>																					\
struct VectorScalarsImpl<T, ScalarsCount>															\
{																									\
	static constexpr size_t Count = ScalarsCount;													\
																									\
	union																							\
	{																								\
		T Scalars[Count];																			\
		struct																						\
		{																							\
			Members																					\
		};																							\
	};																								\
																									\
	constexpr VectorScalarsImpl(const T& scalar = 0)												\
	{																								\
		for (size_t i = 0; i < Count; ++i)															\
		{																							\
			Scalars[i] = scalar;																	\
		}																							\
	}																								\
																									\
	constexpr VectorScalarsImpl(const VectorScalarsImpl& impl)										\
	{																								\
		for (size_t i = 0; i < Count; ++i)															\
		{																							\
			Scalars[i] = impl.Scalars[i];															\
		}																							\
	}																								\
																									\
	template<class... TArgs> requires (sizeof...(TArgs) <= Count && sizeof...(TArgs) >= 2)			\
	constexpr VectorScalarsImpl(const TArgs&... args) : Scalars{ static_cast<T>(args)... }			\
	{																								\
	}																								\
}

DECLARE_VECTOR_SCALARS_IMPL(1, T X;);
DECLARE_VECTOR_SCALARS_IMPL(2, T X; T Y;);
DECLARE_VECTOR_SCALARS_IMPL(3, T X; T Y; T Z;);
DECLARE_VECTOR_SCALARS_IMPL(4, T X; T Y; T Z; T W;);

#undef DECLARE_VECTOR_SCALARS_IMPL