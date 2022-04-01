// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActionResult.h"
#include "HttpStatusCode.h"

namespace libty::Asp::inline RestApi
{
	class ASP_API SObjectResult : public SActionResult
	{
		GENERATED_BODY(SObjectResult);

	private:
		SPROPERTY(_response);
		SObject* _response = nullptr;
		EHttpStatusCode _statusCode;

	protected:
		SObjectResult(SObject* object, EHttpStatusCode statusCode);

		virtual EHttpStatusCode GetStatusCode();
		virtual SObject* GetValue();
	};
}