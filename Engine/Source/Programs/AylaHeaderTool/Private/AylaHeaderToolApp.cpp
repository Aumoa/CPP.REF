// Copyright 2020-2022 Aumoa.lib. All right reserved.

import AylaHeaderTool;

AylaHeaderToolApp::AylaHeaderToolApp()
{
}

int32 AylaHeaderToolApp::Run()
{
	try
	{
		throw TerminateException(TerminateException::EKnownErrorCodes::Success);
	}
	catch (const TerminateException& TE)
	{
		Console::WriteLine(TE.GetMessage());
	}
	return 0;
}