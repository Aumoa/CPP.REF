// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RestApi/ObjectResult.h"

using namespace ::libty;
using namespace ::libty::Asp;
using namespace ::libty::Sockets;

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