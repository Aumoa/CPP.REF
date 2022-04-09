// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/NotImplementedException.h"

using namespace libty;

NotImplementedException::NotImplementedException(const std::source_location& src)
	: FatalException("The function to call does not implemented.", nullptr, src)
{
}