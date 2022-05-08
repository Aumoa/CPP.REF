// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DependencyInjection/IServiceCollection.h"
#include "DependencyInjection/IHostedService.h"
#include "Reflection/Type.h"
#include "Attributes/ClassAttributeAutoRegistration.h"
#include "typeof.h"

using namespace ::libty;

IServiceCollection* IServiceCollection::AddAutoServices()
{
	for (auto& type : SType::GetAllTypes())
	{
		auto* attr = type->GetCustomAttribute<::Attributes::SClassAttributeAutoRegistration>();
		if (attr == nullptr)
		{
			continue;
		}

		switch (attr->ServiceType)
		{
		case EServiceType::HostedService:
			check(type->IsImplemented(typeof(IHostedService)));
			AddHostedService(type);
			break;
		case EServiceType::Singleton:
			AddSingleton(type);
			break;
		case EServiceType::Scoped:
			AddScoped(type);
			break;
		}
	}

	return this;
}