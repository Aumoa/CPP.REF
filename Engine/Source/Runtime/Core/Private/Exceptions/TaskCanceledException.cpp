// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/TaskCanceledException.h"

using namespace libty;

TaskCanceledException::TaskCanceledException(std::exception_ptr innerException)
	: Exception(TEXT("Task was aborted."), innerException)
{
}