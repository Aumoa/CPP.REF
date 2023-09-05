// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "CoreAObject/ClassMetadata.h"

class COREAOBJECT_API AType final : public AObject
{
	friend struct NTypeGen;
	friend class AObject;

protected:
	using Super = AObject;

private:
	String ClassName;
	String PackageName;
	AObject* (*Constructor)() = nullptr;
	EClassMetadata ClassMeta = {};

private:
	AType();
	AType(std::defer_lock_t);

public:
	virtual ~AType() noexcept override;

	String GetClassName() noexcept;

public:
	static AType* StaticClass();
};