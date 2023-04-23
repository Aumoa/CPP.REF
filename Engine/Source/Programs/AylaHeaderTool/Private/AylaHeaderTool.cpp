// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "AylaHeaderTool.h"
#include "Misc/CommandLine.h"
#include "Exceptions/TerminateException.h"
#include "Exceptions/UsageException.h"
#include "Reflection/AAssembly.h"

AylaHeaderTool::AylaHeaderTool()
{
}

Task<int32> AylaHeaderTool::Run(std::stop_token SToken)
{
	try
	{
		if (CommandLine::TryGetValue(TEXT("Usage")))
		{
			PrintUsage(Console::Out);
			co_return 0;
		}

		if (CommandLine::TryGetValue(TEXT("SourceLocation"), SourceLocation) == false)
		{
			throw UsageException(TEXT("SourceLocation"));
		}

		if (Directory::Exists(SourceLocation) == false)
		{
			throw TerminateException(String::Format(TEXT("SourceLocation({}) is not valid directory path."), SourceLocation));
		}

		if (CommandLine::TryGetValue(TEXT("Build"), Build) == false)
		{
			throw UsageException(TEXT("Build"));
		}

		if (CommandLine::TryGetValue(TEXT("Output"), Output) == false)
		{
			throw UsageException(TEXT("Output"));
		}

		if (CommandLine::TryGetValue(TEXT("Includes"), Includes) == false)
		{
			throw UsageException(TEXT("Includes"));
		}

		Directory::CreateDirectory(Build);

		String AssemblyName = Path::GetFileNameWithoutExtension(SourceLocation);
		AAssembly Assembly(AssemblyName);
		Console::WriteLine(TEXT("AylaHeaderTool running on {} module."), AssemblyName);
		auto Timer = PerformanceTimer::StartNew();

		// Create intermediate default directories.
		String GeneratedIntermediatePath = Path::Combine(Includes, AssemblyName);
		Directory::CreateDirectory(GeneratedIntermediatePath);

		std::vector<Task<>> Tasks;
		Spinlock Lock;
		for (auto& File : Directory::GetFiles(SourceLocation, EDirectorySearchOptions::AllDirectories))
		{
			if (Path::GetExtension(File).Equals(TEXT(".h"), EStringComparison::CurrentCultureIgnoreCase))
			{
				Tasks.emplace_back(File::ReadAllTextAsync(File, SToken).ContinueWith([this, &Lock, File, &Assembly, &GeneratedIntermediatePath](auto p)
				{
					if (p.IsCompletedSuccessfully())
					{
						HeaderFile CurrentHeader(File, p.GetResult(), GeneratedIntermediatePath);
						if (CurrentHeader.Parse(Assembly))
						{
							std::unique_lock ScopedLock(Lock);
							HeaderFiles.emplace_back(std::move(CurrentHeader));
						}
					}
					else
					{
						throw TerminateException(String::Format(TEXT("Failed to read header file '{}'."), File));
					}
				}, SToken));
			}
		}

		co_await Task<>::WhenAll(Tasks);

		CSProject.emplace(Build, AssemblyName);

		Tasks.clear();
		for (auto& File : HeaderFiles)
		{
			Tasks.emplace_back(File.SaveAsync(SToken));
		}

		co_await Task<>::WhenAll(Tasks);
		co_await CSProject->GenerateProjectFileAsync(Output, SToken);

		Console::WriteLine(TEXT("Reflection code generated on {} seconds."), Timer.GetElapsed().GetTotalSeconds<double>());
		co_return 0;
	}
	catch (const TerminateException& TE)
	{
		Console::Error.WriteLine(TEXT("Application terminated. {}"), TE.GetMessage());
		co_return 2;
	}
	catch (const UsageException& UE)
	{
		Console::Error.WriteLine(TEXT("Required parameter {} is not provided."), UE.GetParameterName());
		PrintUsage(Console::Error);
		co_return 1;
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
	Writer.WriteLine(TEXT("    -Usage            []          Print usage."));
	Writer.WriteLine(TEXT("    -SourceLocation   [String]    Specify source code location to parse."));
	Writer.WriteLine(TEXT("    -Build            [String]    Specify build location to save reflection code generated."));
	Writer.WriteLine(TEXT("    -Output           [String]    Specify build output location to desire C# project output."));
	Writer.WriteLine(TEXT("    -Includes         [String]    Specify include location to save reflection code generated."));
}