// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/TaskAbortedException.h"

TaskAbortedException::TaskAbortedException(std::exception_ptr innerException)
	: Exception(TEXT("Task was aborted."), innerException)
{
}