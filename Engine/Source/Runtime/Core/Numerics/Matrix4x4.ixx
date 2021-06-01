// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:Matrix4x4;

import std.core;
import :Matrix;

using namespace std;

export struct Matrix4x4 : public Matrix<4, 4>
{
	/// <summary>
	/// Initialize new <see cref="Matrix4x4"/> instance.
	/// </summary>
	constexpr Matrix4x4() : Matrix()
	{
	}

	/// <summary>
	/// Initialize new <see cref="Matrix4x4"/> instance.
	/// </summary>
	/// <param name="values"> The initial values. </param>
	constexpr Matrix4x4(initializer_list<float> values) : Matrix(values)
	{
	}

	/// <summary>
	/// Initialize new <see cref="Matrix4x4"/> instance.
	/// </summary>
	/// <param name="values"> The initial vectors. </param>
	constexpr Matrix4x4(initializer_list<Vector<Column()>> values) : Matrix(values)
	{
	}

	/// <summary>
	/// Get determinant value.
	/// </summary>
	float GetDeterminant() const;

	/// <summary>
	/// Get inversed matrix.
	/// </summary>
	Matrix4x4 GetInverse() const;
};