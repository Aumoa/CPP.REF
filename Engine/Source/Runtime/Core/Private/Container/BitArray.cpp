// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Container/BitArray.h"
#include "CoreAssert.h"

ConstBitIterator::ConstBitIterator()
{
}

ConstBitIterator::ConstBitIterator(const BitArray& bitArray) : _myArr(&bitArray)
{
}

ConstBitIterator& ConstBitIterator::Increment()
{
	_reference.Mask <<= 1;
	++_reference.BitIndex;
	++_dwordCounter;

	if (_dwordCounter == BitArray::NumBitsPerDWORD)
	{
		++_reference.ValueIndex;
		_reference.Mask = 1;
		_dwordCounter = 0;
	}

	return *this;
}

bool ConstBitIterator::IsValid() const
{
	return _reference.BitIndex < _myArr->GetNumBits();
}

BitArray::BitArray()
{
}

BitArray::BitArray(std::initializer_list<bool> initializer)
{
	Init(initializer);
}

BitArray::BitArray(bool bValue, size_t bitsToSet)
{
	Init(bValue, bitsToSet);
}

BitArray::BitArray(const BitArray& rhs)
	: _bits(rhs._bits)
	, _numBits(rhs._numBits)
	, _bitsRaw(_bits.data())
{
}

BitArray::BitArray(BitArray&& rhs) noexcept
	: _bits(move(rhs._bits))
	, _numBits(rhs._numBits)
	, _bitsRaw(_bits.data())
{
}

void BitArray::Init(bool bValue, size_t numBits)
{
	_numBits = numBits;
	_bits.resize(GetValueCount(numBits));
	_bitsRaw = _bits.data();
	memset(_bitsRaw, bValue ? 0xFF : 0, sizeof(int32) * _bits.size());
}

void BitArray::Init(std::initializer_list<bool> initializer)
{
	_numBits = initializer.size();

	size_t values = GetValueCount(_numBits);
	_bits.resize(values, 0);
	_bitsRaw = _bits.data();

	size_t counter = 0;
	const bool* p = initializer.begin();

	for (size_t i = 0; i < _bits.size(); ++i)
	{
		int32& value = _bitsRaw[i];

		int32 mask = 1;
		for (size_t j = 0; j < NumBitsPerDWORD; ++j)
		{
			size_t idx = counter++;
			if (p[idx])
			{
				value |= mask;
			}
			mask <<= 1;
		}
	}
}

void BitArray::SetNumUninitialized(size_t numBits)
{
	_numBits = numBits;
	_bits.resize(GetValueCount(numBits));
	_bitsRaw = _bits.data();
}

auto BitArray::AccessCorrespondingBit(const RelativeBitReference& relativeReference) -> BitReference
{
	return BitReference(this, relativeReference);
}

auto BitArray::AccessCorrespondingBit(const RelativeBitReference& relativeReference) const -> ConstBitReference
{
	return ConstBitReference(this, relativeReference);
}

bool BitArray::Contains(bool bValue) const
{
	if (bValue)
	{
		size_t bits = _numBits;
		for (size_t i = 0; i < _bits.size(); ++i)
		{
			size_t rem = NumBitsPerDWORD - bits;
			int32 mask = (int32)((size_t)1 << (rem + 1)) - 1;
			if ((_bits[i] & mask) != 0)
			{
				return true;
			}
		}

		return false;
	}
	else
	{
		size_t bits = _numBits;
		for (size_t i = 0; i < _bits.size(); ++i)
		{
			size_t rem = NumBitsPerDWORD - bits;
			if ((_bits[i] << rem) != 0)
			{
				return true;
			}
		}

		return false;
	}
}

size_t BitArray::Add(bool bValue)
{
	size_t dwIdx = _numBits % NumBitsPerDWORD;
	if (dwIdx == 0)
	{
		_bits.emplace_back((int32)bValue);
	}
	else
	{
		int32 mask = 1 << dwIdx;
		_bits.back() |= mask;
	}

	return _numBits++;
}

size_t BitArray::Add(bool bValue, size_t numToAdd)
{
	size_t indexOf = AddUninitialized(numToAdd);
	SetRange(indexOf, numToAdd, bValue);
	return indexOf;
}

size_t BitArray::AddUninitialized(size_t numToAdd)
{
	size_t addedIndex = _numBits;
	if (numToAdd > 0)
	{
		size_t OldLastWordIndex = _numBits == 0 ? -1 : (_numBits - 1) / NumBitsPerDWORD;
		size_t NewLastWordIndex = (_numBits + numToAdd - 1) / NumBitsPerDWORD;
		if (NewLastWordIndex == OldLastWordIndex)
		{
			// We're not extending into a new word, so we don't need to reserve more memory and we don't need to clear the unused bits on the final word
			_numBits += numToAdd;
		}
		else
		{
			_bits.resize(_numBits + numToAdd);
			_numBits += numToAdd;
		}
	}
	return addedIndex;
}

void BitArray::SetRange(size_t index, size_t numBitsToSet, bool bValue)
{
	check(index >= 0 && numBitsToSet >= 0 && index + numBitsToSet <= _numBits);

	if (numBitsToSet == 0)
	{
		return;
	}

	// Work out which uint32 index to set from, and how many
	size_t startIndex = index / NumBitsPerDWORD;
	size_t Count = (index + numBitsToSet + (NumBitsPerDWORD - 1)) / NumBitsPerDWORD - startIndex;

	// Work out masks for the start/end of the sequence
	int32 startMask = 0xFFFFFFFF << (index % NumBitsPerDWORD);
	int32 endMask = 0xFFFFFFFF >> (NumBitsPerDWORD - (index + numBitsToSet) % NumBitsPerDWORD) % NumBitsPerDWORD;

	int32* data = _bits.data() + startIndex;
	if (bValue)
	{
		if (Count == 1)
		{
			*data |= startMask & endMask;
		}
		else
		{
			*data++ |= startMask;
			Count -= 2;
			while (Count != 0)
			{
				*data++ = ~0;
				--Count;
			}
			*data |= endMask;
		}
	}
	else
	{
		if (Count == 1)
		{
			*data &= ~(startMask & endMask);
		}
		else
		{
			*data++ &= ~startMask;
			Count -= 2;
			while (Count != 0)
			{
				*data++ = 0;
				--Count;
			}
			*data &= ~endMask;
		}
	}
}

void BitArray::ClearPartialSlackBits()
{
	// BitArray has a contract about bits outside of the active range - the bits in the final word past NumBits are guaranteed to be 0
	// This prevents easy-to-make determinism errors from users of BitArray that do not carefully mask the final word.
	// It also allows us optimize some operations which would otherwise require us to mask the last word.
	const int32 usedBits = _numBits % NumBitsPerDWORD;
	if (usedBits != 0)
	{
		const size_t lastDWORDIndex = _numBits / NumBitsPerDWORD;
		const int32 slackMask = FullDWORDMask >> (NumBitsPerDWORD - usedBits);

		int32* lastDWORD = (GetData() + lastDWORDIndex);
		*lastDWORD = *lastDWORD & slackMask;
	}
}