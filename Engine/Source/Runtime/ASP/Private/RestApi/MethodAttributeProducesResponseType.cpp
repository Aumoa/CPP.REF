// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RestApi/MethodAttributeProducesResponseType.h"

using namespace libty::Asp;

SMethodAttributeProducesResponseType::SMethodAttributeProducesResponseType(EHttpStatusCode statusCode)
	: _statusCode(statusCode)
{
}

EHttpStatusCode SMethodAttributeProducesResponseType::GetProducesCode()
{
	return _statusCode;
}