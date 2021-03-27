// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "NotSupportedException.h"

NotSupportedException::NotSupportedException() : Super(L"Feature is not supported.")
{

}

NotSupportedException::NotSupportedException(TRefPtr<String> message) : Super(message)
{

}

NotSupportedException::NotSupportedException(TRefPtr<String> message, Exception* innerException) : Super(message, innerException)
{

}

NotSupportedException::~NotSupportedException()
{

}