// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class COREAOBJECT_API ObjectReference
{
	AObject* sourcePointer = nullptr;
	std::atomic<bool> lived;
	std::atomic<size_t> weaks;

public:
	inline ObjectReference(AObject* sourcePointer) noexcept
		: sourcePointer(sourcePointer)
		, lived(true)
		, weaks(1)
	{
		check(sourcePointer);
	}

	inline void ReleaseWeak() noexcept
	{
		if (--weaks == 0)
		{
			delete this;
		}
	}

	inline bool IsLive() const noexcept
	{
		return lived;
	}

	inline size_t GetWeaks() const noexcept
	{
		return weaks.load();
	}

	static bool Destroy(ObjectReference& reference)
	{
		// already destroyed, or invalid reference.
		if (reference.sourcePointer == nullptr)
		{
			return false;
		}

		bool expected = true;
		if (reference.lived.compare_exchange_strong(expected, false))
		{
			delete reference.sourcePointer;
			reference.sourcePointer = nullptr;
			reference.ReleaseWeak();
			return true;
		}

		// already destroyed.
		return false;
	}
};