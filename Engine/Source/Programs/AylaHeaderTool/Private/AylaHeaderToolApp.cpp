// Copyright 2020-2022 Aumoa.lib. All right reserved.

import AylaHeaderTool;

AylaHeaderToolApp::AylaHeaderToolApp()
{
}

int32 AylaHeaderToolApp::Run()
{
	std::vector<Task<>> Tasks;
	for (int32 i = 0; i < 32; ++i)
	{
		Tasks.emplace_back(Task<>::Run([]()
		{
			Console::WriteLine(TEXT("[{}] In worker thread."), PlatformMisc::GetThreadName());
		}));
	}

	try
	{
		if (CommandLine::Contains(TEXT("help")))
		{
			PrintUsage(Console::GetOut());
			return 0;
		}

		throw TerminateException(TerminateException::EKnownErrorCodes::Usage);
	}
	catch (const TerminateException& TE)
	{
		Console::WriteLine(TE.ToString());
		if (TE.ErrorCode == TerminateException::EKnownErrorCodes::Usage)
		{
			PrintUsage(Console::GetError());
		}

		return (int32)TE.ErrorCode;
	}

	Task<>::WhenAll(Tasks).GetResult();
	return 0;
}

void AylaHeaderToolApp::PrintUsage(TextWriter& Output)
{
	Output.WriteLine(TEXT("Usage: "));
}