// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CodeGen/TypeGen.h"
#include "Type.h"
#include "CoreAObject/TypeCollection.h"

NTypeGen::NCompiledInDefer::NCompiledInDefer(AType* ClassType, String PackageName)
{
	ClassType->PackageName = PackageName;
	NTypeCollection::AddClassTypeToCollection(ClassType);
}

void NTypeGen::GenerateClassType(AType*& ConstructTo, const NClassMetadata& Metadata)
{
	if (Metadata.ClassName == TEXT("Type"))
	{
		ConstructTo = new AType(std::defer_lock);
	}
	else
	{
		ConstructTo = new AType();
	}
	ConstructTo->ClassName = Metadata.ClassName;
	ConstructTo->Constructor = Metadata.Constructor;
	ConstructTo->ClassMeta = Metadata.ClassMeta;
}