// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"

class AType;

class COREAOBJECT_API AObject
{
private:
	struct Referencer
	{
		int32 Refs;
		int32 Weaks;

		void IncrRef() noexcept
		{
			PlatformAtomics::InterlockedIncrement(&Weaks);
			PlatformAtomics::InterlockedIncrement(&Refs);
		}

		bool DecrRef() noexcept
		{
			const bool bDangling = PlatformAtomics::InterlockedDecrement(&Refs) == 0;
			if (PlatformAtomics::InterlockedDecrement(&Weaks) == 0)
			{
				delete this;
			}
			return bDangling;
		}
	};

	struct GCCollectorMarks
	{
		bool bMarkReferenced : 1 = false;
	};

private:
	Referencer* Refs = nullptr;
	GCCollectorMarks GCM;

public:
	AObject();
	virtual ~AObject() noexcept;

	virtual String ToString();
	virtual AType* GetType();
};