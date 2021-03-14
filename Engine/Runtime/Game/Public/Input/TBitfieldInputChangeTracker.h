// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

template<class TBitfield>
struct TBitfieldInputChangeTracker
{
private:
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

	Packer32Bits pressed;
	Packer32Bits released;
	Packer32Bits last;
	
public:
	TBitfieldInputChangeTracker();

	void Update(const Packer32Bits& newState);
	void Reset();
	vs_property_get_auto(TBitfield, Last, (const TBitfield&)last);

	bool IsPressedByBitIndex(uint32 bitIndex) const;
	bool IsReleasedByBitIndex(uint32 bitIndex) const;
};

#include "TBitfieldInputChangeTracker.inl"