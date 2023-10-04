// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class AType;
class APackage;

struct COREAOBJECT_API NTypeCollection : public StaticClass
{
	friend struct NTypeGen;

	static std::vector<APackage*> GetPackages();

private:
	static void AddClassTypeToCollection(AType* InClassType);
};