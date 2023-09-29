// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "CoreAObject/ClassMetadata.h"

class COREAOBJECT_API AType final : public AObject
{
	friend struct NTypeGen;
	friend class AObject;

public:
	using Super = AObject;

private:
	String ClassName;
	String PackageName;
	AType* SuperClass = nullptr;
	AObject* (*Constructor)() = nullptr;
	EClassMetadata ClassMeta = {};

private:
	AType();
	AType(std::defer_lock_t);

public:
	virtual ~AType() noexcept override;

	String GetClassName() const noexcept;
	bool IsDerivedFrom(AType* InType) const noexcept;
	template<std::derived_from<AObject> U>
	bool IsDerivedFrom() const noexcept { return IsDerivedFrom(U::StaticClass()); }

public:
	static AType* StaticClass();
};