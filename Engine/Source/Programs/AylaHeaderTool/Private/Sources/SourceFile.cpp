// Copyright 2020-2023 Aumoa.lib. All right reserved.

import AylaHeaderTool;

SourceFile::SourceFile(String InPath)
	: SourcePath(InPath)
{
}

String SourceFile::GetSourcePath() const
{
	return SourcePath;
}