// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DependencyInjection/ServiceCollectionExtensions.h"
#include "DependencyInjection/IServiceCollection.h"
#include "Controllers/ControllerBase.h"
#include "RestApi/MethodAttributeHttpGet.h"
#include "RestApi/MethodAttributeProducesResponseType.h"
#include "Controllers/ClassAttributeRoute.h"

using namespace libty::Asp;

IServiceCollection* SServiceCollectionExtensions::AddControllers(IServiceCollection* collection)
{
	auto concatPath = [](std::wstring_view lhs, std::wstring_view rhs) -> std::wstring
	{
		if (lhs.empty() && !rhs.empty())
		{
			return std::wstring(rhs);
		}
		else if (rhs.empty() && !lhs.empty())
		{
			return std::wstring(lhs);
		}
		else
		{
			return std::wstring(lhs) + L"/"s + std::wstring(rhs);
		}
	};

	auto* baseType = typeof(SControllerBase);
	for (auto& controllerClass : SType::GetDerivedTypes(baseType))
	{
		if (!controllerClass->IsA(baseType))
		{
			collection = collection->AddController(controllerClass);
			std::wstring baseRoute = controllerClass->GetCustomAttribute<SClassAttributeRoute>()->GetRoute();

			for (auto& method : controllerClass->GetMethods())
			{
				for (auto& attr : method->GetAttributes())
				{
					if (attr->GetType()->IsA<SMethodAttributeHttpGet>())
					{
						auto* ptr = Cast<SMethodAttributeHttpGet>(attr);
					}
					else if (attr->GetType()->IsA<SMethodAttributeProducesResponseType>())
					{
						auto* ptr = Cast<SMethodAttributeProducesResponseType>(attr);
					}
				}
			}
		}
	}

	return collection;
}