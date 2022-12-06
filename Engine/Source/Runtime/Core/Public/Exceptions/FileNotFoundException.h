// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

class CORE_API FileNotFoundException : public Exception
{
	using Super = Exception;

public:
	FileNotFoundException(String filename, std::exception_ptr innerException = nullptr);

private:
	static String FormatMessage(String filename);
};