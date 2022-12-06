// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/FileNotFoundException.h"

FileNotFoundException::FileNotFoundException(String filename, std::exception_ptr innerException)
	: Super(FormatMessage(filename), innerException)
{
}

String FileNotFoundException::FormatMessage(String filename)
{
	if (filename.IsEmpty())
	{
		return TEXT("File not found.");
	}
	else
	{
		return String::Format(TEXT("Desired file({}) not found."), filename);
	}
}