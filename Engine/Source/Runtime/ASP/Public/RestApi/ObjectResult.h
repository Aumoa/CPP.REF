// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "ActionResult.h"
#include "Web/HttpStatusCode.h"

namespace libty::inline Asp
{
	class ASP_API SObjectResult : extends(SActionResult)
	{
		GENERATED_BODY(SObjectResult);

	private:
		SPROPERTY(_response);
		SObject* _response = nullptr;
		Sockets::EHttpStatusCode _statusCode;

	protected:
		SObjectResult(SObject* object, Sockets::EHttpStatusCode statusCode);

		virtual Sockets::EHttpStatusCode GetStatusCode();
		virtual SObject* GetValue();
	};
}