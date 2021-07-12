// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "BitArray.h"

using namespace std;

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

	if (_dwordCounter == 32)
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

BitArray::BitArray(initializer_list<bool> initializer)
{
	Init(initializer);
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

void BitArray::Init(initializer_list<bool> initializer)
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
		for (size_t j = 0; j < 32; ++j)
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