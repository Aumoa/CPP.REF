// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "System/Version.h"
#include "System/ArgumentException.h"

Version::Version(String InVersionStr)
{
	if (TryParse(InVersionStr, *this) == false)
	{
		throw ArgumentException(TEXT("Invalid version format."));
	}
}

bool Version::TryParse(String InVersionStr, Version& OutVersion)
{
	size_t MajorEnd = InVersionStr.IndexOf('.');
	if (MajorEnd == IntegralTypes::Npos)
	{
		return false;
	}

	if (Int32::TryParse(InVersionStr.SubstringView(0, MajorEnd), OutVersion.Major) == false)
	{
		return false;
	}

	String Context = InVersionStr.SubstringView(MajorEnd + 1);
	size_t MinorEnd = Context.IndexOf('.');
	if (Int32::TryParse(Context.SubstringView(0, MinorEnd), OutVersion.Minor) == false)
	{
		return false;
	}

	if (MinorEnd == IntegralTypes::Npos)
	{
		return true;
	}

	Context = Context.SubstringView(MinorEnd + 1);
	size_t BuildEnd = Context.IndexOf('.');
	if (Int32::TryParse(Context.SubstringView(0, BuildEnd), OutVersion.Build) == false)
	{
		return false;
	}

	if (BuildEnd == IntegralTypes::Npos)
	{
		return true;
	}

	return Int32::TryParse(Context.SubstringView(BuildEnd + 1), OutVersion.Revision);
}