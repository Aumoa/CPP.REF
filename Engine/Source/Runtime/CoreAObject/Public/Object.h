// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"

class AType;
class NObjectInitializer;

template<class T>
class RefPtr;

class COREAOBJECT_API AObject
{
	template<class T>
	friend class RefPtr;

private:
	AType* const ClassType = nullptr;
	Referencer* Refs = nullptr;

protected:
	AObject();
	AObject(NObjectInitializer& Initializer);

public:
	virtual ~AObject() noexcept;

	virtual String ToString();
	AType* GetType();

	FORCEINLINE int32 GetLocks() const noexcept { return Refs->GetLocks(); }
	FORCEINLINE int32 GetWeaks() const noexcept { return Refs->GetWeaks(); }

public:
	static AType* StaticClass();
	static RefPtr<AObject> NewObject(AType* InClassType);
};

#include "RefPtr.h"