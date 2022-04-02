// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RestApi/MethodAttributeHttpGet.h"

using namespace ::Attributes;

SMethodAttributeHttpGet::SMethodAttributeHttpGet(std::wstring_view route)
	: _route(route)
{
}

std::wstring SMethodAttributeHttpGet::GetRoute()
{
	return _route;
}