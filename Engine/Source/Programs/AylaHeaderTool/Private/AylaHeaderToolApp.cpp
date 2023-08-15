// Copyright 2020-2022 Aumoa.lib. All right reserved.

import AylaHeaderTool;

AylaHeaderToolApp::AylaHeaderToolApp()
{
}

Task<int32> AylaHeaderToolApp::RunConsoleAsync(std::stop_token InCancellationToken)
{
	try
	{
		if (CommandLine::Contains(TEXT("help")))
		{
			PrintUsage(Console::GetOut());
			co_return 0;
		}

		ParseCommandLines();

		std::vector<Task<>> Tasks;
		for (auto& Path : Directory::GetFiles(SourcePath, ESearchOption::AllDirectories))
		{
			if (Path::GetExtension(Path) == TEXT(".ixx"))
			{
				std::unique_ptr<SourceFile>& Source = Sources.emplace_back(std::make_unique<InterfaceSource>(Path));
				Tasks.emplace_back(Source->TryParseAsync(InCancellationToken));
			}
		}

		co_await Task<>::WhenAll(Tasks);
	}
	catch (const TerminateException& TE)
	{
		Log::Critical(TE.ToString());
		Log::FlushAll();

		if (TE.ErrorCode == TerminateException::EKnownErrorCodes::Usage)
		{
			PrintUsage(Console::GetError());
		}

		co_return (int32)TE.ErrorCode;
	}

	co_return 0;
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