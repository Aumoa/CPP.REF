// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RestApi/ObjectResult.h"

GENERATE_BODY(libty::Asp::RestApi::SObjectResult);

using namespace libty::Asp::RestApi;

SObjectResult::SObjectResult(SObject* object, EHttpStatusCode statusCode)
	: Super()
	, _response(object)
	, _statusCode(statusCode)
{
}

EHttpStatusCode SObjectResult::GetStatusCode()
{
	return _statusCode;
}

SObject* SObjectResult::GetValue()
{
	return _response;
}