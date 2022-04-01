// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RestApi/OkObjectResult.h"

GENERATE_BODY(libty::Asp::RestApi::SOkObjectResult);

using namespace libty::Asp::RestApi;

SOkObjectResult::SOkObjectResult(SObject* value)
	: Super(value, EHttpStatusCode::OK)
{
}