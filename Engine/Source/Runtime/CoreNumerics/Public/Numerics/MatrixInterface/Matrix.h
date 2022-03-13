// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Numerics/VectorInterface/Vector.h"

template<class T = void, size_t NRow = 0, size_t NCol = 0>
struct Matrix
{
	Vector<T, NCol> V[NRow];

private:
	struct Private
	{
	};

public:
	template<class... TArgs>
	constexpr Matrix(TArgs&&... Args) requires
		(sizeof...(TArgs) > 1 && sizeof...(TArgs) <= NRow * NCol) &&
		std::constructible_from<Matrix, int, TArgs...> &&
		(!SameAsVariadic<Private, 0, TArgs...>)
		: Matrix(Private{}, 0, std::forward<TArgs>(Args)...)
	{
	}

	constexpr Matrix(const Matrix& Rhs) : V{}
	{
		for (size_t i = 0; i < NRow; ++i)
		{
			V[i] = Rhs.V[i];
		}
	}

	template<TIsMatrix<T, NRow, NCol> IMatrix>
	constexpr Matrix(const IMatrix& M) : V{}
	{
		for (size_t i = 0; i < NRow; ++i)
		{
			for (size_t j = 0; j < NCol; ++j)
			{
				V[i][j] = M[i][j];
			}
		}
	}

	constexpr Matrix& operator =(const Matrix& M)
	{
		for (size_t i = 0; i < NRow; ++i)
		{
			for (size_t j = 0; j < NCol; ++j)
			{
				V[i][j] = M[i][j];
			}
		}
		return *this;
	}

private:
	template<class... TArgs>
	constexpr Matrix(Private&&, int, TArgs&&... Args) requires (sizeof...(TArgs) == NRow * NCol) && requires
	{
		{ std::initializer_list<T>{ std::declval<TArgs>()... } };
	}
	: Matrix(Private{}, MakeVectors<0>(std::make_tuple(std::forward<TArgs>(Args)...), std::make_index_sequence<NCol>{}))
	{
	}

	template<class TVectorTuple, size_t... Indexes>
	constexpr Matrix(Private&&, TVectorTuple&& Args, std::index_sequence<Indexes...>&&)
		: Matrix(Private{}, 0, std::get<Indexes>(Args)...)
	{
	}

	template<class... TArgs>
	constexpr Matrix(Private&&, short, TArgs&&... Args) requires (sizeof...(TArgs) == NRow) && requires
	{
		{ std::initializer_list<Vector<T, NCol>>{ std::declval<TArgs>() } };
	}
	: V{ std::forward<TArgs>(Args)... }
	{
	}

	template<size_t Index, class TTuple, size_t... Indexes>
	constexpr auto MakeVectors(TTuple&& Args, std::index_sequence<Indexes...>&&)
	{
		if constexpr (Index == NRow - 1)
		{
			return std::make_tuple(VectorType(std::get<Indexes>(Args)...));
		}
		else
		{
			return std::tuple_cat(
				std::make_tuple(VectorType(std::get<Indexes>(Args)...)),
				MakeVectors<Index + 1>(std::move(Args), std::index_sequence<Indexes...>{})
			);
		}
	}

public:
	using Type = T;
	using VectorType = Vector<T, NCol>;

	constexpr Matrix(const VectorType& S = {}) : V{}
	{
		for (size_t i = 0; i < NRow; ++i)
		{
			V[i] = S;
		}
	}

	static constexpr size_t Row()
	{
		return NRow;
	}

	static constexpr size_t Column()
	{
		return NCol;
	}

	constexpr Matrix operator -() const
	{
		Matrix M;
		for (size_t i = 0; i < NRow; ++i)
		{
			M[i] = -V[i];
		}
		return M;
	}

	constexpr const auto& operator [](size_t N) const
	{
		return V[N];
	}

	constexpr auto& operator [](size_t N)
	{
		return V[N];
	}

	template<TIsMatrix<T, NRow, NCol> IMatrix>
	constexpr Matrix& operator =(const IMatrix& M)
	{
		for (size_t i = 0; i < NRow; ++i)
		{
			for (size_t j = 0; j < NCol; ++j)
			{
				V[i][j] = M[i][j];
			}
		}
		return *this;
	}

	static constexpr Matrix Identity()
	{
		Matrix M;
		for (size_t i = 0; i < NRow; ++i)
		{
			for (size_t j = 0; j < NCol; ++j)
			{
				M[i][j] = (T)(i == j);
			}
		}
		return M;
	}

public:
	template<TIsMatrix<T, NRow, NCol> IMatrix>
	constexpr bool NearlyEquals(const IMatrix& M, const T& Epsilon) const;
	inline std::wstring ToString(std::wstring_view FormatArgs) const;
};


template<>
struct Matrix<void, 0, 0>
{
	template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
	static constexpr bool NearlyEquals(const IMatrixL& ML, IMatrixR& MR, const typename IMatrixL::Type& Epsilon) requires TIsCompatibleMatrix<IMatrixL, IMatrixR>
	{
		static constexpr size_t Row = ML.Row();
		static constexpr size_t Col = ML.Column();

		for (size_t i = 0; i < Row; ++i)
		{
			for (size_t j = 0; j < Col; ++j)
			{
				if (MathEx::Abs(ML[i][j] - MR[i][j]) > Epsilon)
				{
					return false;
				}
			}
		}

		return true;
	}

	template<TIsMatrixBase IMatrix>
	static constexpr bool IsSquared(const IMatrix& M)
	{
		return IsSquared<IMatrix>();
	}

	template<TIsMatrixBase IMatrix>
	static constexpr bool IsSquared()
	{
		return IMatrix::Row() == IMatrix::Column();
	}

	template<TIsMatrixBase IMatrix>
	static constexpr bool IsIdentity(const IMatrix& M, const typename IMatrix::Type& Epsilon = {})
	{
		using T = typename IMatrix::Type;
		for (size_t i = 0; i < M.Row(); ++i)
		{
			for (size_t j = 0; j < M.Column(); ++j)
			{
				if (MathEx::Abs(M[i][j] - (T)(i == j)) > Epsilon)
				{
					return false;
				}
			}
		}
		return true;
	}

	template<TIsMatrixBase IMatrix>
	static constexpr auto Transpose(const IMatrix& M)
	{
		Matrix<typename IMatrix::Type, IMatrix::Column(), IMatrix::Row()> R;
		for (size_t i = 0; i < IMatrix::Row(); ++i)
		{
			for (size_t j = 0; j < IMatrix::Column(); ++j)
			{
				R[j][i] = M[i][j];
			}
		}
		return R;
	}

	template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
	static constexpr auto Multiply(const IMatrixL& ML, const IMatrixR& MR) requires
		(IMatrixR::Row() >= IMatrixL::Column()) &&
		std::same_as<typename IMatrixL::Type, typename IMatrixR::Type>
	{
		Matrix<typename IMatrixL::Type, IMatrixL::Row(), IMatrixR::Column()> R;
		for (size_t i = 0; i < IMatrixL::Row(); ++i)
		{
			for (size_t j = 0; j < IMatrixR::Column(); ++j)
			{
				R[i][j] = {};
				for (size_t k = 0; k < IMatrixL::Column(); ++k)
				{
					R[i][j] += ML[i][k] * MR[k][j];
				}
			}
		}
		return R;
	}

	template<TIsMatrixBase IMatrix, TIsVectorBase IVector>
	static constexpr auto TransformVector(const IMatrix& M, const IVector& V) requires
		std::same_as<typename IMatrix::Type, typename IVector::Type> &&
		(IMatrix::Row() == IVector::Size())
	{
		using T = typename IMatrix::Type;
		Vector<T, IMatrix::Column()> R;

		for (size_t j = 0; j < IMatrix::Column(); ++j)
		{
			for (size_t i = 0; i < IMatrix::Row(); ++i)
			{
				R[j] += M[i][j] * V[i];
			}
		}

		return R;
	}

	template<TIsMatrixBase IMatrix>
	static constexpr auto Minor(const IMatrix& M, size_t NRow, size_t NCol) requires (IMatrix::Row() > 1 && IMatrix::Column() > 1)
	{
		Matrix<typename IMatrix::Type, IMatrix::Row() - 1, IMatrix::Column() - 1> R(0.0f);
		for (size_t i = 0; i < IMatrix::Row(); ++i)
		{
			if (i < NRow)
			{
				R[i] = Vector<>::Minor(M[i], NCol);
			}
			else if (i > NRow)
			{
				R[i - 1] = Vector<>::Minor(M[i], NCol);
			}
		}
		return R;
	}

	template<TIsMatrixBase IMatrix>
	static constexpr float Determinant(const IMatrix& M)
	{
		static_assert(IsSquared<IMatrix>(), "Matrix is not squared.");

		if constexpr (IMatrix::Row() == 1)
		{
			return (float)M[0][0];
		}
		else
		{
			float Result = 0;
			for (size_t i = 0; i < IMatrix::Column(); ++i)
			{
				float Mul = (i % 2) == 0 ? +1.0f : -1.0f;
				Result += Mul * M[0][i] * Determinant(Minor(M, 0, i));
			}
			return Result;
		}
	}

	template<TIsMatrixBase IMatrix>
	static constexpr auto Cofactor(const IMatrix& M)
	{
		Matrix<float, IMatrix::Row(), IMatrix::Column()> R(0.0f);
		for (size_t i = 0; i < IMatrix::Row(); ++i)
		{
			for (size_t j = 0; j < IMatrix::Column(); ++j)
			{
				float Mul = ((i + j) % 2) == 0 ? +1.0f : -1.0f;
				R[i][j] = Mul * Determinant(Minor(M, i, j));
			}
		}

		return R;
	}

	template<TIsMatrixBase IMatrix>
	static constexpr auto Adjoint(const IMatrix& M)
	{
		return Transpose(Cofactor(M));
	}

	template<TIsMatrixBase IMatrix>
	static constexpr auto Inverse(const IMatrix& M)
	{
		static_assert(IsSquared<IMatrix>(), "Matrix is not squared.");

		auto Det = Determinant(M);
		return (1.0f / Det) * Adjoint(M);
	}

	template<TIsMatrixBase IMatrix>
	static std::wstring ToString(const IMatrix& M, std::wstring_view FormatArgs = L"")
	{
		std::array<std::wstring, IMatrix::Row()> Composed;
		for (size_t i = 0; i < IMatrix::Row(); ++i)
		{
			Composed[i] = M[i].ToString(FormatArgs);
		}

		return std::format(L"{{{}}}", String::Join(L", ", std::span<std::wstring const>(Composed)));
	}

	template<TIsMatrixBase IMatrix>
	static constexpr auto AdjustIdentity(const IMatrix& M, const typename IMatrix::Type& Epsilon)
	{
		if (IsIdentity(M, Epsilon))
		{
			return IMatrix::Identity();
		}
		else
		{
			return M;
		}
	}
};


template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
constexpr IMatrixL operator +(const IMatrixL& ML, const IMatrixR& MR) requires TIsCompatibleMatrix<IMatrixL, IMatrixR>
{
	IMatrixL R;
	for (size_t i = 0; i < ML.Row(); ++i)
	{
		R[i] = ML[i] + MR[i];
	}
	return R;
}

template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
constexpr IMatrixL operator -(const IMatrixL& ML, const IMatrixR& MR) requires TIsCompatibleMatrix<IMatrixL, IMatrixR>
{
	IMatrixL R;
	for (size_t i = 0; i < ML.Row(); ++i)
	{
		R[i] = ML[i] - MR[i];
	}
	return R;
}

template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
constexpr IMatrixL operator *(const IMatrixL& ML, const IMatrixR& MR) requires TIsCompatibleMatrix<IMatrixL, IMatrixR>
{
	IMatrixL R;
	for (size_t i = 0; i < ML.Row(); ++i)
	{
		R[i] = ML[i] * MR[i];
	}
	return R;
}

template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
constexpr IMatrixL operator /(const IMatrixL& ML, const IMatrixR& MR) requires TIsCompatibleMatrix<IMatrixL, IMatrixR>
{
	IMatrixL R;
	for (size_t i = 0; i < ML.Row(); ++i)
	{
		R[i] = ML[i] / MR[i];
	}
	return R;
}

template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
constexpr IMatrixL operator %(const IMatrixL& ML, const IMatrixR& MR) requires TIsCompatibleMatrix<IMatrixL, IMatrixR>
{
	IMatrixL R;
	for (size_t i = 0; i < ML.Row(); ++i)
	{
		R[i] = ML[i] % MR[i];
	}
	return R;
}
template<TIsMatrixBase IMatrix>
constexpr IMatrix operator *(const IMatrix& ML, const typename IMatrix::Type& S)
{
	IMatrix R;
	for (size_t i = 0; i < ML.Row(); ++i)
	{
		R[i] = ML[i] * S;
	}
	return R;
}

template<TIsMatrixBase IMatrix>
constexpr IMatrix operator /(const IMatrix& ML, const typename IMatrix::Type& S)
{
	IMatrix R;
	for (size_t i = 0; i < ML.Row(); ++i)
	{
		R[i] = ML[i] / S;
	}
	return R;
}

template<TIsMatrixBase IMatrix>
constexpr IMatrix operator %(const IMatrix& ML, const typename IMatrix::Type& S)
{
	IMatrix R;
	for (size_t i = 0; i < ML.Row(); ++i)
	{
		R[i] = ML[i] % S;
	}
	return R;
}

template<TIsMatrixBase IMatrix>
constexpr auto operator *(const typename IMatrix::Type& S, const IMatrix& V)
{
	return IMatrix(S) * V;
}

template<TIsMatrixBase IMatrix>
constexpr auto operator /(const typename IMatrix::Type& S, const IMatrix& V)
{
	return IMatrix(S) / V;
}

template<TIsMatrixBase IMatrix>
constexpr auto operator %(const typename IMatrix::Type& S, const IMatrix& V)
{
	return IMatrix(S) % V;
}

template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
constexpr auto operator ^(const IMatrixL& ML, const IMatrixR& MR) requires TIsCompatibleMatrix<IMatrixL, IMatrixR> && requires
{
	{ Matrix<>::Multiply(ML, MR) };
}
{
	return Matrix<>::Multiply(ML, MR);
}


template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
decltype(auto) operator +=(IMatrixL& V, const IMatrixR& VR) requires TIsCompatibleMatrix<IMatrixL, IMatrixR> && std::assignable_from<IMatrixL, IMatrixL>
{
	return V = V + VR;
}

template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
decltype(auto) operator -=(IMatrixL& V, const IMatrixR& VR) requires TIsCompatibleMatrix<IMatrixL, IMatrixR> && std::assignable_from<IMatrixL, IMatrixL>
{
	return V = V - VR;
}

template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
decltype(auto) operator *=(IMatrixL& V, const IMatrixR& VR) requires TIsCompatibleMatrix<IMatrixL, IMatrixR> && std::assignable_from<IMatrixL, IMatrixL>
{
	return V = V * VR;
}

template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
decltype(auto) operator /=(IMatrixL& V, const IMatrixR& VR) requires TIsCompatibleMatrix<IMatrixL, IMatrixR> && std::assignable_from<IMatrixL, IMatrixL>
{
	return V = V / VR;
}

template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
decltype(auto) operator %=(IMatrixL& V, const IMatrixR& VR) requires TIsCompatibleMatrix<IMatrixL, IMatrixR> && std::assignable_from<IMatrixL, IMatrixL>
{
	return V = V % VR;
}

template<TIsMatrixBase IMatrix>
decltype(auto) operator *=(IMatrix& V, const typename IMatrix::Type& S) requires std::assignable_from<IMatrix, IMatrix>
{
	return V = V * S;
}

template<TIsMatrixBase IMatrix>
decltype(auto) operator /=(IMatrix& V, const typename IMatrix::Type& S) requires std::assignable_from<IMatrix, IMatrix>
{
	return V = V / S;
}

template<TIsMatrixBase IMatrix>
decltype(auto) operator %=(IMatrix& V, const typename IMatrix::Type& S) requires std::assignable_from<IMatrix, IMatrix>
{
	return V = V % S;
}


template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
constexpr std::strong_ordering operator <=>(const IMatrixL& ML, const IMatrixR& MR) requires TIsCompatibleMatrix<IMatrixL, IMatrixR>
{
	for (size_t i = 0; i < IMatrixL::Row(); ++i)
	{
		for (size_t j = 0; j < IMatrixL::Column(); ++j)
		{
			const auto R = ML[i][j] <=> MR[i][j];

			if (R < 0)
			{
				return std::strong_ordering::less;
			}
			else if (R > 0)
			{
				return std::strong_ordering::greater;
			}
		}
	}

	return std::strong_ordering::equal;
}

template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
constexpr bool operator < (const IMatrixL& ML, const IMatrixR& MR) requires TIsCompatibleMatrix<IMatrixL, IMatrixR>
{
	return operator <=>(ML, MR) < 0;
}

template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
constexpr bool operator <=(const IMatrixL& ML, const IMatrixR& MR) requires TIsCompatibleMatrix<IMatrixL, IMatrixR>
{
	return operator <=>(ML, MR) <= 0;
}

template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
constexpr bool operator > (const IMatrixL& ML, const IMatrixR& MR) requires TIsCompatibleMatrix<IMatrixL, IMatrixR>
{
	return operator <=>(ML, MR) > 0;
}

template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
constexpr bool operator >=(const IMatrixL& ML, const IMatrixR& MR) requires TIsCompatibleMatrix<IMatrixL, IMatrixR>
{
	return operator <=>(ML, MR) >= 0;
}

template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
constexpr bool operator ==(const IMatrixL& ML, const IMatrixR& MR) requires TIsCompatibleMatrix<IMatrixL, IMatrixR>
{
	return operator <=>(ML, MR) == 0;
}

template<TIsMatrixBase IMatrixL, TIsMatrixBase IMatrixR>
constexpr bool operator !=(const IMatrixL& ML, const IMatrixR& MR) requires TIsCompatibleMatrix<IMatrixL, IMatrixR>
{
	return operator <=>(ML, MR) != 0;
}


template<class T, size_t NRow, size_t NCol>
template<TIsMatrix<T, NRow, NCol> IMatrix>
constexpr bool Matrix<T, NRow, NCol>::NearlyEquals(const IMatrix& M, const T& Epsilon) const
{
	return Matrix<>::NearlyEquals(*this, M, Epsilon);
}

template<class T, size_t NRow, size_t NCol>
inline std::wstring Matrix<T, NRow, NCol>::ToString(std::wstring_view FormatArgs) const
{
	return Matrix<>::ToString(*this, FormatArgs);
}