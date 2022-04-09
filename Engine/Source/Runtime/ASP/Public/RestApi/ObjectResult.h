// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "ActionResult.h"
#include "Web/HttpStatusCode.h"

namespace libty::Asp::inline RestApi
{
	class ASP_API SObjectResult : public SActionResult
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