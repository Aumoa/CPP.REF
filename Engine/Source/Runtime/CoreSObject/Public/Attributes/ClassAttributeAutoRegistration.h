// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "ClassAttribute.h"
#include "DependencyInjection/ServiceType.h"

namespace Attributes::inline Core
{
	class CORESOBJECT_API SClassAttributeAutoRegistration : public ::libty::SClassAttribute
	{
		GENERATED_BODY(SClassAttributeAutoRegistration);

	public:
		SClassAttributeAutoRegistration(::libty::EServiceType serviceType = ::libty::EServiceType::Singleton);

	public:
		const ::libty::EServiceType ServiceType;
	};
}