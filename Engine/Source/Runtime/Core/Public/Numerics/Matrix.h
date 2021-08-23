// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Vector.h"

/// <summary>
/// Represents matrix contains NRow count vectors that have NCol components.
/// </summary>
template<size_t NRow, size_t NCol>
struct Matrix
{
	/// <summary>
	/// The vector list.
	/// </summary>
	Vector<NCol> V[NRow];

	/// <summary>
	/// Initialize new <see cref="Matrix"/> instance.
	/// </summary>
	constexpr Matrix()
	{
	}

	/// <summary>
	/// Initialize new <see cref="Matrix"/> instance.
	/// </summary>
	/// <param name="values"> The initial values. </param>
	constexpr Matrix(std::initializer_list<float> values)
	{
		for (size_t i = 0; i < MathEx::Max(values.size(), NRow * NCol); ++i)
		{
			V[i / NCol].Scalars[i % NCol] = values.begin()[i];
		}
	}

	/// <summary>
	/// Initialize new <see cref="Matrix"/> instance.
	/// </summary>
	/// <param name="values"> The initial vectors. </param>
	constexpr Matrix(std::initializer_list<Vector<NCol>> values)
	{
		for (size_t i = 0; i < MathEx::Max(values.size(), NRow); ++i)
		{
			V[i] = values.begin()[i];
		}
	}

	constexpr bool NearlyEquals(const Matrix& rhs, float epsilon = MathEx::SmallNumber) const
	{
		for (size_t i = 0; i < NRow; ++i)
		{
			if (!V[i].NearlyEquals(rhs.V[i], epsilon))
			{
				return false;
			}
		}

		return true;
	}

	/// <summary>
	/// Get row count.
	/// </summary>
	static constexpr size_t Row()
	{
		return NRow;
	}

	/// <summary>
	/// Get column count.
	/// </summary>
	static constexpr size_t Column()
	{
		return NCol;
	}

	/// <summary>
	/// Get component.
	/// </summary>
	/// <param name="row"> The row index. </param>
	/// <param name="column"> The column index. </param>
	/// <returns> Component value. </returns>
	constexpr const float& Get(size_t row, size_t column) const
	{
		return V[row][column];
	}

	/// <summary>
	/// Get component.
	/// </summary>
	/// <param name="row"> The row index. </param>
	/// <param name="column"> The column index. </param>
	/// <returns> Component value. </returns>
	float& Get(size_t row, size_t column)
	{
		return V[row][column];
	}

	/// <summary>
	/// Represents the matrix is square matrix.
	/// </summary>
	static constexpr bool IsSquared()
	{
		return NRow == NCol;
	}

	/// <summary>
	/// Represents the matrix is identity.
	/// </summary>
	constexpr bool IsIdentity() const
	{
		if constexpr (IsSquared())
		{
			for (size_t i = 0; i < NRow; ++i)
			{
				for (size_t j = 0; j < NCol; ++j)
				{
					if (V[i][j] != (float)(i == j))
					{
						return false;
					}
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	/// <summary>
	/// Get transposed matrix.
	/// </summary>
	/// <returns></returns>
	constexpr Matrix<NCol, NRow> GetTransposed() const
	{
		Matrix<NCol, NRow> m;
		for (size_t i = 0; i < NRow; ++i)
		{
			for (size_t j = 0; j < NCol; ++j)
			{
				m.V[j][i] = V[i][j];
			}
		}
		return m;
	}

	/// <summary>
	/// Multiply two matrices.
	/// </summary>
	/// <param name="lhs"></param>
	/// <param name="rhs"></param>
	/// <returns></returns>
	template<size_t TCol>
	static constexpr Matrix<NRow, TCol> Multiply(const Matrix& lhs, const Matrix<NCol, TCol>& rhs)
	{
		Matrix<NRow, TCol> m;
		for (size_t i = 0; i < NRow; ++i)
		{
			for (size_t j = 0; j < TCol; ++j)
			{
				m.V[i].Scalars[j] = 0;
				for (size_t k = 0; k < NCol; ++k)
				{
					m.V[i].Scalars[j] += lhs.V[i].Scalars[k] * rhs.V[k].Scalars[j];
				}
			}
		}
		return m;
	}

	constexpr bool operator ==(const Matrix& rhs) const
	{
		for (size_t i = 0; i < NRow; ++i)
		{
			for (size_t j = 0; j < NCol; ++j)
			{
				if (V[i][j] != rhs.V[i][j])
				{
					return false;
				}
			}
		}
		return true;
	}

	constexpr bool operator !=(const Matrix& rhs) const
	{
		for (size_t i = 0; i < NRow; ++i)
		{
			for (size_t j = 0; j < NCol; ++j)
			{
				if (V[i][j] != rhs.V[i][j])
				{
					return true;
				}
			}
		}
		return false;
	}

	std::wstring ToString(std::wstring_view formatArgs) const
	{
		std::wstring placeholder = StringUtils::GetPlaceholder(formatArgs);

		std::array<std::wstring, NRow> composed;
		for (size_t i = 0; i < NRow; ++i)
		{
			composed[i] = V[i].ToString(formatArgs);
		}

		return std::format(L"{{{}}}", StringUtils::Join(L", ", std::span<std::wstring const>(composed)));
	}
};