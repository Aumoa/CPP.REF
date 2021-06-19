// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

template<class TBitfield>
class BitfieldInputChangeTracker
{
	struct alignas(32) Packer32Bits : public TBitfield
	{
	public:
		Packer32Bits() : TBitfield({ })
		{
		}

		Packer32Bits(const TBitfield& rhs) : TBitfield(rhs)
		{
		}
	};

	Packer32Bits pressed = {};
	Packer32Bits released = {};
	Packer32Bits last = {};

public:
	BitfieldInputChangeTracker()
	{
	}

	void Update(const Packer32Bits& newState)
	{
		auto currPtr = reinterpret_cast<const uint32*>(&newState);
		auto prevPtr = reinterpret_cast<const uint32*>(&last);
		auto releasedPtr = reinterpret_cast<uint32*>(&released);
		auto pressedPtr = reinterpret_cast<uint32*>(&pressed);

		// Fast-check changes of state using 32-bit unit.
		for (size_t i = 0; i < (sizeof(TBitfield) / 4); ++i)
		{
			*pressedPtr = *currPtr & ~(*prevPtr);
			*releasedPtr = ~(*currPtr) & *prevPtr;

			++currPtr;
			++prevPtr;
			++releasedPtr;
			++pressedPtr;
		}

		// Slow-check changes of state using remainder.
		if (size_t rem = sizeof(TBitfield) % 32; rem != 0)
		{
			int32 curr = *currPtr;
			int32 prev = *prevPtr;

			*pressedPtr = curr & ~prev;
			*releasedPtr = ~curr & prev;
		}

		last = newState;
	}

	void Reset()
	{
		memset(&pressed, 0, sizeof(pressed));
		memset(&released, 0, sizeof(released));
	}

	const TBitfield& GetLast() const
	{
		return (const TBitfield&)last;
	}

	bool IsPressedByBitIndex(uint32 bitIndex) const
	{
		auto ptr = reinterpret_cast<const uint32*>(&pressed);
		unsigned int bf = 1u << bitIndex;
		return (ptr[(bitIndex >> 5)] & bf) != 0;
	}

	const uint32& GetPressedBitIndex(uint32 bitIndex) const
	{
		auto ptr = reinterpret_cast<const uint32*>(&pressed);
		return ptr[bitIndex];
	}

	bool IsReleasedByBitIndex(uint32 bitIndex) const
	{
		auto ptr = reinterpret_cast<const uint32*>(&released);
		unsigned int bf = 1u << bitIndex;
		return (ptr[(bitIndex >> 5)] & bf) == 0;
	}

	const uint32& GetReleasedBitIndex(uint32 bitIndex) const
	{
		auto ptr = reinterpret_cast<const uint32*>(&released);
		return ptr[bitIndex];
	}
};