// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "AylaHeaderTool.h"
#include "Misc/CommandLine.h"
#include "Exceptions/TerminateException.h"
#include "Exceptions/UsageException.h"

AylaHeaderTool::AylaHeaderTool()
{
}

int32 AylaHeaderTool::Run()
{
	try
	{
		if (CommandLine::TryGetValue(TEXT("Usage")))
		{
			PrintUsage(Console::Out);
			return 0;
		}

		if (CommandLine::TryGetValue(TEXT("SourceLocation"), SourceLocation) == false)
		{
			throw UsageException(TEXT("SourceLocation"));
		}

		if (Directory::Exists(SourceLocation) == false)
		{
			throw TerminateException(String::Format(TEXT("SourceLocation({}) is not valid directory path."), SourceLocation));
		}

		return 0;
	}
	catch (const TerminateException& TE)
	{
		Console::Error.WriteLine(TEXT("Application terminated. {}"), TE.GetMessage());
		return 2;
	}
	catch (const UsageException& UE)
	{
		Console::Error.WriteLine(TEXT("Required parameter {} is not provided."), UE.GetParameterName());
		PrintUsage(Console::Error);
		return 1;
	}
}

void AylaHeaderTool::PrintUsage(TextWriter& Writer)
{
#if PLATFORM_WINDOWS
	String ExecutableName = TEXT("AylaHeaderTool.exe");
#else
#error Unsupported platform.
#endif

	Writer.WriteLine(TEXT("Usage: "));
	Writer.WriteLine(TEXT("  {}"), ExecutableName);
	Writer.WriteLine(TEXT("    -Usage         [?]          Print usage."));
}