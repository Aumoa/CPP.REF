// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Linq/Generator.h"
#include "Exceptions/Exception.h"

void Linq::GeneratorHelper::_Xthrow_already_start()
{
	throw Exception(TEXT("Already started."));
}

void Linq::GeneratorHelper::_Xthrow_value_null()
{
	throw Exception(TEXT("Value is null."));
}

void Linq::GeneratorHelper::_Xthrow_coro_done()
{
	throw Exception(TEXT("Coroutine is done."));
}