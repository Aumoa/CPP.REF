// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Linq/Generator.h"
#include "Exceptions/Exception.h"

template<>
void Linq::Generator<void>::_Xthrow_already_start()
{
	throw Exception(TEXT("Already started."));
}

template<>
void Linq::Generator<void>::_Xthrow_value_null()
{
	throw Exception(TEXT("Value is null."));
}

template<>
void Linq::Generator<void>::_Xthrow_coro_done()
{
	throw Exception(TEXT("Coroutine is done."));
}