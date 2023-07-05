// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Numerics:VectorScalarsImpl;

export import Core;

#define VECTOR_SCALARS_CONSTEXPR_CTORS \
	constexpr VectorScalarsImpl(const T& S = 0) : Scalars{} \
	{ \
		for (size_t i = 0; i < Count; ++i) \
		{ \
			Scalars[i] = S; \
		} \
	} \
	\
	constexpr VectorScalarsImpl(const VectorScalarsImpl& Impl) \
	{ \
		for (size_t i = 0; i < Count; ++i) \
		{ \
			Scalars[i] = Impl.Scalars[i]; \
		} \
	} \
	\
	template<class... TArgs> \
	constexpr VectorScalarsImpl(const TArgs&... Args) \
		requires (sizeof...(TArgs) >= Count) && \
		requires { std::initializer_list<T>{ static_cast<T>(Args)... }; } \
		: VectorScalarsImpl(std::make_tuple(Args...), std::make_index_sequence<Count>{}) \
	{ \
	} \
	\
private: \
	template<class TTuple, size_t... Indexes> \
	constexpr VectorScalarsImpl(TTuple&& Args, std::index_sequence<Indexes...>&&) \
		requires requires { std::initializer_list<T>{ static_cast<T>(std::get<Indexes>(Args))... }; } \
		: Scalars{ static_cast<T>(std::get<Indexes>(Args))... } \
	{ \
	}

	template<class T, size_t N>
	struct VectorScalarsImpl
	{
		static_assert(N != 0, "!(N == 0)");

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

		VECTOR_SCALARS_CONSTEXPR_CTORS;
	};

#define DECLARE_VECTOR_SCALARS_IMPL(ScalarsCount, Members)	\
template<class T>											\
struct VectorScalarsImpl<T, ScalarsCount>					\
{															\
	static constexpr size_t Count = ScalarsCount;			\
															\
	union													\
	{														\
		T Scalars[Count];									\
		struct												\
		{													\
			Members											\
		};													\
	};														\
															\
	VECTOR_SCALARS_CONSTEXPR_CTORS;							\
}

export
{
	DECLARE_VECTOR_SCALARS_IMPL(1, T X;);
	DECLARE_VECTOR_SCALARS_IMPL(2, T X; T Y;);
	DECLARE_VECTOR_SCALARS_IMPL(3, T X; T Y; T Z;);
}

#undef DECLARE_VECTOR_SCALARS_IMPL
#undef VECTOR_SCALARS_CONSTEXPR_CTORS