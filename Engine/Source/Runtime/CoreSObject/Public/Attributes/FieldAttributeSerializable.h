// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "FieldAttribute.h"
#include "Reflection/TypeInfoMetadataGenerator.h"
#include "Reflection/TypeInfoMetadataGenerator.Impl.h"

namespace Attributes::inline Core
{
	class CORESOBJECT_API SFieldAttributeSerializable : public ::libty::Core::Attributes::SFieldAttribute
	{
		GENERATED_BODY(SFieldAttributeSerializable);

	public:
		SFieldAttributeSerializable();
	};
}