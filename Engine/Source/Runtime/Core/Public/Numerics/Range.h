// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

/// <summary>
/// Represents range that defined by a start index and end index.
/// </summary>
struct Range
{
	size_t Start = 0;
	size_t End = 0;

	/// <summary>
	/// Initialize new <see cref="Range"/> instance.
	/// </summary>
	constexpr Range()
	{
	}

	/// <summary>
	/// Initialize new <see cref="Range"/> instance.
	/// </summary>
	/// <param name="start"> The start index. </param>
	/// <param name="end"> The end index. </param>
	constexpr Range(size_t start, size_t end)
	{
		Start = start;
		End = end;
	}

	/// <summary>
	/// Get length of this range.
	/// </summary>
	/// <returns> The length. </returns>
	inline constexpr size_t GetLength() const
	{
		return End - Start;
	}
};