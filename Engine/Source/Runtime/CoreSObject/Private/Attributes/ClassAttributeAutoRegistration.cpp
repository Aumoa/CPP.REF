// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Attributes/ClassAttributeAutoRegistration.h"

using namespace ::libty;

namespace Attributes
{
	SClassAttributeAutoRegistration::SClassAttributeAutoRegistration(EServiceType serviceType)
		: Super()
		, ServiceType(serviceType)
	{
	}
}