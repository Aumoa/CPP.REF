// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Controllers/ControllerBase.h"
#include "RestApi/OkObjectResult.h"

GENERATE_BODY(libty::Asp::Controllers::SControllerBase);

using namespace libty::Asp;

SControllerBase::SControllerBase()
	: Super()
{
}

SOkObjectResult* SControllerBase::Ok(SObject* value)
{
	return gcnew SOkObjectResult(value);
}