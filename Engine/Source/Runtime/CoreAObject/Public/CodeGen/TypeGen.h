// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CoreAObject/ClassMetadata.h"

class AType;
class AObject;

struct COREAOBJECT_API NTypeGen : public StaticClass
{
	struct NClassMetadata
	{
		String ClassName;
		AObject* (*Constructor)();
		EClassMetadata ClassMeta;
	};

	struct COREAOBJECT_API NCompiledInDefer
	{
		NCompiledInDefer(AType* ClassType, String PackageName, AType* SuperClassType);
	};

	static void GenerateClassType(AType*& ConstructTo, const NClassMetadata& Metadata);
};