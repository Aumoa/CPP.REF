// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:Matrix;

import std.core;
import :Vector;
import :MathEx;

using namespace std;

export
{
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
		constexpr Matrix(initializer_list<float> values)
		{
			for (size_t i = 0; i < MathEx::Max(values.size(), NRow * NCol); ++i)
			{
				V[i / NCol].Values[i % NCol] = values.begin()[i];
			}
		}

		/// <summary>
		/// Initialize new <see cref="Matrix"/> instance.
		/// </summary>
		/// <param name="values"> The initial vectors. </param>
		constexpr Matrix(initializer_list<Vector<NCol>> values)
		{
			for (size_t i = 0; i < MathEx::Max(values.size(), NRow); ++i)
			{
				V[i] = values.begin()[i];
			}
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
		template<enable_if_t<IsSquared(), void*> = nullptr>
		constexpr bool IsIdentity() const
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

		/// <summary>
		/// Get transposed matrix.
		/// </summary>
		/// <returns></returns>
		template<enable_if_t<IsSquared(), void*> = nullptr>
		constexpr Matrix GetTransposed() const
		{
			Matrix m;
			for (size_t i = 0; i < NRow; ++i)
			{
				for (size_t j = 0; j < NCol; ++j)
				{
					m.V[j][i] = V[i][j];
				}
			}
			return m;
		}
	};
}