// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RestApi/MethodAttributeHttpGet.h"

GENERATE_BODY(libty::Asp::RestApi::SMethodAttributeHttpGet);

using namespace libty::Asp::RestApi;

SMethodAttributeHttpGet::SMethodAttributeHttpGet(std::wstring_view route)
	: _route(route)
{
}

std::wstring SMethodAttributeHttpGet::GetRoute()
{
	return _route;
}