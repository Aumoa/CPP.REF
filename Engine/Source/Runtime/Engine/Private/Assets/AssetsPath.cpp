// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Assets/AssetsPath.h"

String NAssetsPath::GetFileSystemPath(String InLogicalPath)
{
	String FileSystemPath;
	String LogicalPathView = InLogicalPath;

	if (LogicalPathView.StartsWith(TEXT("/Engine/")))
	{
		FileSystemPath = Path::Combine(Environment::GetEngineDirectory(), TEXT("Content"));
		LogicalPathView = InLogicalPath.SubstringView(TEXT("/Engine/").length());
	}
	else
	{
		checkf(false, TEXT("Invalid logical path detected."));
		return String::GetEmpty();
	}

	LogicalPathView = Path::Combine(LogicalPathView.Replace(DirectorySeparatorString, Char::ToStringView(Path::DirectorySeparatorChar)));
	return Path::Combine(FileSystemPath, LogicalPathView);
}