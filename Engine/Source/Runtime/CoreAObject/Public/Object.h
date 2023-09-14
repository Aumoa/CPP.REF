// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"

class AType;
class NObjectInitializer;

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

	AType* const ClassType = nullptr;

protected:
	AObject();
	AObject(NObjectInitializer& Initializer);

public:
	virtual ~AObject() noexcept;

	virtual String ToString();
	AType* GetType();

public:
	static AType* StaticClass();
	static std::shared_ptr<AObject> NewObject(AType* InClassType);
};

template<std::derived_from<AObject> UObject>
inline std::shared_ptr<UObject> NewObject(AType* InClassType = nullptr)
{
	if (InClassType == nullptr)
	{
		InClassType = UObject::StaticClass();
	}

	std::shared_ptr<AObject> Instanced = AObject::NewObject(InClassType);
	std::shared_ptr<UObject> Casted = std::dynamic_pointer_cast<UObject>(Instanced);
	check(Casted);
	return Casted;
}