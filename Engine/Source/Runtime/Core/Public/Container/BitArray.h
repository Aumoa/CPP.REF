// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <vector>
#include "PrimitiveTypes.h"

class BitArray;

struct CORE_API RelativeBitReference
{
	size_t ValueIndex = 0;
	int32 Mask = 1;
	size_t BitIndex = 0;
};

class CORE_API ConstBitIterator
{
	const BitArray* _myArr = nullptr;
	RelativeBitReference _reference = {};
	uint8 _dwordCounter = 0;

public:
	ConstBitIterator();
	ConstBitIterator(const BitArray& bitArray);

	ConstBitIterator& Increment();
	bool IsValid() const;

	inline RelativeBitReference GetIndex() const { return _reference; }

	inline ConstBitIterator& operator ++() { return Increment(); }
	inline operator bool() const { return IsValid(); }
};

/// <summary>
/// A dynamically sized bit array. An array of Booleans. They stored in one bit/Boolean. There are iterators that efficiently iterate over only set bits.
/// </summary>
class CORE_API BitArray
{
public:
	static constexpr size_t NumBitsPerDWORD = 32;
	static constexpr int32 FullDWORDMask = 0xFFFFFFFF;

private:
	std::vector<int32> _bits;
	int32* _bitsRaw = nullptr;
	size_t _numBits = 0;

public:
	struct BitReference : public RelativeBitReference
	{
		using Super = RelativeBitReference;

		BitArray* Array = nullptr;

		BitReference(BitArray* inArray, const RelativeBitReference& bitReference) : RelativeBitReference(bitReference)
			, Array(inArray)
		{
		}

		BitReference& operator =(bool bValue)
		{
			if (bValue)
			{
				Array->_bitsRaw[ValueIndex] |= Mask;
			}
			else
			{
				Array->_bitsRaw[ValueIndex] &= ~Mask;
			}
			return *this;
		}

		operator bool() const
		{
			return (Array->_bitsRaw[ValueIndex] & Mask) != 0;
		}
	};

	struct ConstBitReference : public RelativeBitReference
	{
		using Super = RelativeBitReference;

		const BitArray* Array = nullptr;

		ConstBitReference(const BitArray* inArray, const RelativeBitReference& bitReference) : RelativeBitReference(bitReference)
			, Array(inArray)
		{
		}

		operator bool() const
		{
			return (Array->_bitsRaw[ValueIndex] & Mask) != 0;
		}
	};

public:
	BitArray();
	BitArray(std::initializer_list<bool> initializer);
	BitArray(bool bValue, size_t bitsToSet);
	BitArray(const BitArray& rhs);
	BitArray(BitArray&& rhs) noexcept;

	void Init(bool bValue, size_t numBits);
	void Init(std::initializer_list<bool> initializer);
	void SetNumUninitialized(size_t numBits);

	BitReference AccessCorrespondingBit(const RelativeBitReference& relativeReference);
	ConstBitReference AccessCorrespondingBit(const RelativeBitReference& relativeReference) const;

	bool Contains(bool bValue) const;
	size_t Add(bool bValue);
	size_t Add(bool bValue, size_t numToAdd);
	size_t AddUninitialized(size_t numToAdd);
	void SetRange(size_t index, size_t numBitsToSet, bool bValue);
	inline size_t GetNumBits() const { return _numBits; }
	
	const int32* GetData() const { return _bits.data(); }
	int32* GetData() { return _bits.data(); }

private:
	inline size_t GetValueCount(size_t n)
	{
		if (n == 0)
		{
			return 0;
		}
		else
		{
			return (n - 1) / 32 + 1;
		}
	}

	void ClearPartialSlackBits();
};