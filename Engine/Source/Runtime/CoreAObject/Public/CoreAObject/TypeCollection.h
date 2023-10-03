// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class AType;

struct COREAOBJECT_API NTypeCollection : public StaticClass
{
	friend struct NTypeGen;

private:
	static void AddClassTypeToCollection(AType* InClassType);
};