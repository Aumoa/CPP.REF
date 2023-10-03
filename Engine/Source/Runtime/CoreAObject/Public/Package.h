// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "CoreAObject/ClassMetadata.h"

class COREAOBJECT_API APackage final : public AObject
{
	friend class AObject;
	friend struct NTypeCollection;

public:
	using Super = AObject;

private:
	String PackageName;
	std::vector<AType*> PackageTypes;

private:
	APackage();

public:
	virtual ~APackage() noexcept override;

	inline String GetName() const noexcept { return PackageName; }
	inline std::span<AType* const> GetTypes() const noexcept { return PackageTypes; }

	static AType* StaticClass();
};