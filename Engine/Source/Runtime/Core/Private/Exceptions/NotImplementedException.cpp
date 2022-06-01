// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/NotImplementedException.h"

NotImplementedException::NotImplementedException()
	: Exception(TEXT("The function to call does not implemented."))
{
}