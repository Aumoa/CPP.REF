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

		throw TerminateException(TerminateException::EKnownErrorCodes::Usage);
	}
	catch (const TerminateException& TE)
	{
		Console::WriteLine(TE.GetMessage());
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
}