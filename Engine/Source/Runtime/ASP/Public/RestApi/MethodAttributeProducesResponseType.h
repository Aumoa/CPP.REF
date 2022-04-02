// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Web/HttpStatusCode.h"

namespace Attributes::inline libty::inline Asp
{
	class ASP_API SMethodAttributeProducesResponseType : public ::libty::SMethodAttribute
	{
		GENERATED_BODY(SMethodAttributeProducesResponseType);

	private:
		::libty::Sockets::EHttpStatusCode _statusCode;

	public:
		SMethodAttributeProducesResponseType(::libty::Sockets::EHttpStatusCode statusCode);

		::libty::Sockets::EHttpStatusCode GetProducesCode();
	};
}