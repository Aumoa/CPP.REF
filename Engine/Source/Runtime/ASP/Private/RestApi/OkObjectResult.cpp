// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RestApi/OkObjectResult.h"

using namespace libty::Asp;
using namespace libty::Sockets;

SOkObjectResult::SOkObjectResult(SObject* value)
	: Super(value, EHttpStatusCode::OK)
{
}