// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include <utility>

template<size_t N>
class BitSet
{
private:
	static constexpr size_t CalcBytes(size_t require)
	{
		return (require - 1) / 8 + 1;
	}

private:
	static constexpr size_t CountBytes = CalcBytes(N);
	int8 _binaryArray[CountBytes];

public:
	constexpr BitSet() : _binaryArray{}
	{
	}

	template<class... TArgs> requires (sizeof...(TArgs) <= N)
	constexpr BitSet(TArgs&&... args) : BitSet(std::make_index_sequence<CountBytes>{}, std::forward<TArgs>(args)...)
	{
	}

private:
	template<class... TArgs, size_t... _Seq>
	constexpr BitSet(std::index_sequence<_Seq...>&&, TArgs&&... args)
		: _binaryArray{ BitsToByte<_Seq>(((bool)std::forward<TArgs>(args))...)... }
	{
	}

	template<size_t _Idx, class... TArgs>
	constexpr static int8 BitsToByte(
		bool b1 = false,
		bool b2 = false,
		bool b3 = false,
		bool b4 = false,
		bool b5 = false,
		bool b6 = false,
		bool b7 = false,
		bool b8 = false,
		TArgs&&... args)
	{
		if constexpr (_Idx == 0)
		{
			return (b1 ? 1 : 0) << 0
				| (b2 ? 1 : 0) << 1
				| (b3 ? 1 : 0) << 2
				| (b4 ? 1 : 0) << 3
				| (b5 ? 1 : 0) << 4
				| (b6 ? 1 : 0) << 5
				| (b7 ? 1 : 0) << 6
				| (b8 ? 1 : 0) << 7;
		}
		else
		{
			return BitsToByte<_Idx - 1>(std::forward<TArgs>(args)...);
		}
	}

private:
	constexpr inline size_t GetBinaryIndex(size_t index) const
	{
		return index / 8;
	}

	constexpr inline size_t GetBitIndex(size_t index) const
	{
		return index % 8;
	}

public:
	constexpr bool GetBit(size_t bitIndex) const
	{
		return (_binaryArray[GetBinaryIndex(bitIndex)] >> GetBitIndex(bitIndex)) == (int8)1;
	}

	void SetBit(size_t bitIndex, bool bValue)
	{
		const int8 BitValue = (int8)1 << GetBitIndex(bitIndex);

		if (bValue)
		{
			_binaryArray[GetBinaryIndex(bitIndex)] |= BitValue;
		}
		else
		{
			_binaryArray[GetBinaryIndex(bitIndex)] &= ~BitValue;
		}
	}
};