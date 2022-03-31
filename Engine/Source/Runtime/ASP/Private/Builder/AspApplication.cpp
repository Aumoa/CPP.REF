// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Builder/AspApplication.h"
#include "Builder/AspApplicationBuilder.h"

GENERATE_BODY(libty::Asp::Builder::SAspApplication);

using namespace libty::Asp::Builder;

SAspApplication::SAspApplication()
	: Super()
{
}

SAspApplicationBuilder* SAspApplication::CreateBuilder(const CommandLine& args)
{
	return gcnew SAspApplicationBuilder();
}