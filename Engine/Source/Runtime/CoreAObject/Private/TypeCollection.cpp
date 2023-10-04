// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreAObject/TypeCollection.h"
#include "Type.h"
#include "Package.h"
#include "CoreAObject/ObjectInitializer.h"

inline namespace TypeCollectionVars
{
	static std::map<String, std::unique_ptr<APackage>>& Packages()
	{
		static std::map<String, std::unique_ptr<APackage>> StaticInitVar;
		return StaticInitVar;
	}
}

std::vector<APackage*> NTypeCollection::GetPackages()
{
	return Packages() | Linq::Select(LP1R(p, p.second.get())) | Linq::ToVector();
}

void NTypeCollection::AddClassTypeToCollection(AType* InClassType)
{
	auto It = Packages().find(InClassType->PackageName);
	if (It == Packages().end())
	{
		if (InClassType->ClassName == TEXT("Package"))
		{
			NObjectInitializer::Get().MarkInit(InClassType);
		}
		else
		{
			NObjectInitializer::Get().MarkInit(APackage::StaticClass());
		}

		auto Pkg = std::unique_ptr<APackage>(new APackage());
		Pkg->PackageName = InClassType->PackageName;
		It = Packages().emplace(InClassType->PackageName, std::move(Pkg)).first;
	}

	It->second->PackageTypes.emplace_back(InClassType);
	InClassType->Package = It->second.get();
}