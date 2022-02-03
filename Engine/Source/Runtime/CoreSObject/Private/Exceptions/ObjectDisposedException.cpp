// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/ObjectDisposedException.h"

GENERATE_BODY(SObjectDisposedException);

SObjectDisposedException::SObjectDisposedException()
	: Super(L"Cannot access a disposed object.")
{
}

SObjectDisposedException::SObjectDisposedException(std::wstring_view ObjectName)
	: Super(std::format(L"Cannot access a disposed '{}' object.", ObjectName))
{
}