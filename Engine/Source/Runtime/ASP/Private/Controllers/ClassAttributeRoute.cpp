// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Controllers/ClassAttributeRoute.h"

using namespace ::libty;
using namespace ::Attributes;

SClassAttributeRoute::SClassAttributeRoute(std::wstring_view route)
	: Super()
	, _route(route)
{
}

std::wstring SClassAttributeRoute::GetRoute()
{
	return String::ReplaceAll(_route, L"[controller]", GetBindType()->GetName().substr(1));
}