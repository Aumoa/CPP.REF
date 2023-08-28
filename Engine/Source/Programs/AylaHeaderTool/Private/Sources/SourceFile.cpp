// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Sources/SourceFile.h"

SourceFile::SourceFile(String InPath)
	: SourcePath(InPath)
{
}

String SourceFile::GetSourcePath() const
{
	return SourcePath;
}