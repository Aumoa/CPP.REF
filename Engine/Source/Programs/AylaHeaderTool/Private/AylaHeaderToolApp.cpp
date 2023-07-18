// Copyright 2020-2022 Aumoa.lib. All right reserved.

import AylaHeaderTool;

AylaHeaderToolApp::AylaHeaderToolApp()
{
}

Task<> ConfigureAwaitTestAsync(size_t Number)
{
	co_await Task<>::Yield();

	auto Id = std::this_thread::get_id();
	Log::Information(TEXT("[{}] = [{}] In worker thread."), Number, PlatformMisc::GetThreadName());

	co_await Task<>::Yield().ConfigureAwait(true);
	Log::Information(TEXT("[{}] = [{}] In worker thread."), Number, PlatformMisc::GetThreadName());
	check(Id == std::this_thread::get_id());

	co_await Task<>::Yield().ConfigureAwait(true);
	Log::Information(TEXT("[{}] = [{}] In worker thread."), Number, PlatformMisc::GetThreadName());
	check(Id == std::this_thread::get_id());
}

int32 AylaHeaderToolApp::Run()
{
	std::vector<Task<>> Tasks;
	for (int32 i = 0; i < 32; ++i)
	{
		Tasks.emplace_back(ConfigureAwaitTestAsync(i));
	}

	Task<>::WhenAll(Tasks).GetResult();

	Console::CancelKeyPressed() += []()
	{
		Log::Critical(TEXT("Cancel key pressed."));
		Log::FlushAll();
		__debugbreak();
	};

	String Temp = Console::ReadLine();

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
}