// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/TaskCanceledException.h"

using namespace libty;

TaskCanceledException::TaskCanceledException(std::exception_ptr innerException, std::source_location src)
	: FatalException("Task was aborted.", innerException, src)
{
}