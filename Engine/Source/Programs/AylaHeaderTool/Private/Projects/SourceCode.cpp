// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Projects/SourceCode.h"
#include "IO/File.h"

SourceCode::SourceCode(String InPath, String InSourceText)
	: Path(InPath)
	, SourceText(InSourceText)
{
}

SourceCode::~SourceCode() noexcept
{
}

String SourceCode::GetSourcePath() const
{
	return Path;
}

String SourceCode::GetSource() const
{
	return SourceText;
}

Task<> SourceCode::SaveAsync(std::stop_token SToken)
{
	co_await File::WriteAllTextAsync(GetSourcePath(), GetSource(), 0, SToken);
}

Task<> SourceCode::CompareAndSaveAsync(String InPath, String InText, uint32 Encoding, std::stop_token SToken)
{
	String Previous;
	if (File::Exists(InPath))
	{
		Previous = co_await File::ReadAllTextAsync(InPath, SToken);
	}

	if (Previous != InText)
	{
		co_await File::WriteAllTextAsync(InPath, InText, Encoding, SToken);
	}
}