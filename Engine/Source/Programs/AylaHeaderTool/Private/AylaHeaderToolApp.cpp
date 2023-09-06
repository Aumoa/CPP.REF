// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "AylaHeaderToolApp.h"
#include "Exceptions/TerminateException.h"
#include "Sources/SourceFile.h"
#include "Sources/HeaderSource.h"

AylaHeaderToolApp::AylaHeaderToolApp()
{
}

Task<int32> AylaHeaderToolApp::RunConsoleAsync(std::stop_token InCancellationToken)
{
	try
	{
		if (CommandLine::Contains(TEXT("help")))
		{
			PrintUsage(Console::Out);
			co_return 0;
		}

		ParseCommandLines();

		std::vector<Task<bool>> Tasks;
		for (auto& Path : Directory::GetFiles(SourcePath, ESearchOption::AllDirectories))
		{
			if (Path::GetExtension(Path) == TEXT(".h"))
			{
				std::unique_ptr<SourceFile>& Source = Sources.emplace_back(std::make_unique<HeaderSource>(Path));
				Tasks.emplace_back(Source->TryParseAsync(InCancellationToken));
			}
		}

		std::vector<bool> Results = co_await Task<>::WhenAll(Tasks);
		if (Results | Linq::Contains(false))
		{
			Console::Error.WriteLine(TEXT("Parsing error!"));
			co_return 1;
		}

		Tasks.clear();
		for (auto& Source : Sources)
		{
			Tasks.emplace_back(Source->CompileAsync(InCancellationToken));
		}

		Results = co_await Task<>::WhenAll(Tasks);
		if (Results | Linq::Contains(false))
		{
			Console::Error.WriteLine(TEXT("Compile error!"));
			co_return 1;
		}

		std::vector<Task<>> Waits;
		Waits.reserve(Sources.size());
		for (auto& Source : Sources)
		{
			Waits.emplace_back(Source->GenerateAsync(IncludesPath, InCancellationToken));
		}

		co_await Task<>::WhenAll(Waits);
	}
	catch (const TerminateException& TE)
	{
		Console::WriteLine(TE.ToString());

		if (TE.ErrorCode == TerminateException::EKnownErrorCodes::Usage)
		{
			PrintUsage(Console::Error);
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