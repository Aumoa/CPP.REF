// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/TaskAbortedException.h"

using namespace libty;

TaskAbortedException::TaskAbortedException(std::exception_ptr innerException)
	: FatalException("Task was aborted.", innerException)
{
}