// Copyright 2020-2022 Aumoa.lib. All right reserved.

import AylaHeaderTool;

AylaHeaderToolApp::AylaHeaderToolApp()
{
}

int32 AylaHeaderToolApp::Run()
{
	try
	{
		if (CommandLine::Contains(TEXT("help")))
		{
			PrintUsage(Console::GetOut());
			return 0;
		}

		ParseCommandLines();

		for (auto& Path : Directory::GetFiles(SourcePath, ESearchOption::AllDirectories))
		{
			Console::WriteLine(Path);
		}
	}
	catch (const TerminateException& TE)
	{
		Log::Critical(TE.ToString());
		Log::FlushAll();

		if (TE.ErrorCode == TerminateException::EKnownErrorCodes::Usage)
		{
			PrintUsage(Console::GetError());
		}

		return (int32)TE.ErrorCode;
	}

	return 0;
}

void AylaHeaderToolApp::PrintUsage(TextWriter& Output)
{
	Output.WriteLine(TEXT("Usage: "));
	Output.WriteLine(TEXT("  -Source [String] The source code path."));
	Output.WriteLine(TEXT("  -Includes [String] The include path to save generated header files."));
}

void AylaHeaderToolApp::ParseCommandLines()
{
	if (CommandLine::TryGetValue(TEXT("source"), SourcePath) == false || SourcePath.IsEmpty())
	{
		throw TerminateException(TerminateException::EKnownErrorCodes::Usage);
	}

	if (CommandLine::TryGetValue(TEXT("includes"), IncludesPath) == false || IncludesPath.IsEmpty())
	{
		throw TerminateException(TerminateException::EKnownErrorCodes::Usage);
	}
}