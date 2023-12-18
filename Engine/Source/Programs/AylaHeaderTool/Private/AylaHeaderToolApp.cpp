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

		if (Directory::Exists(IncludesPath) == false)
		{
			Directory::CreateDirectory(IncludesPath);
		}

		if (Directory::Exists(CSharpPath) == false)
		{
			Directory::CreateDirectory(CSharpPath);
		}

		std::vector<Task<bool>> Tasks;
		for (auto& Path : Directory::GetFiles(SourcePath, ESearchOption::AllDirectories))
		{
			if (Path::GetExtension(Path) == TEXT(".h"))
			{
				std::unique_ptr<SourceFile>& Source = Sources.emplace_back(std::make_unique<HeaderSource>(PackageName, Path));
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
			Waits.emplace_back(Source->GenerateAsync(IncludesPath, CSharpPath, InCancellationToken));
		}

		co_await Task<>::WhenAll(Waits);

		String CSharpProject = TEXT(R"(
<Project Sdk="Microsoft.NET.Sdk">

  <PropertyGroup>
    <TargetFramework>net8.0</TargetFramework>
    <ImplicitUsings>enable</ImplicitUsings>
    <Nullable>enable</Nullable>
    <RootNamespace>AE.{0}</RootNamespace>
    <OutputPath>$(SolutionDir)Binaries\Interop</OutputPath>
    <AppendTargetFrameworkToOutputPath>false</AppendTargetFrameworkToOutputPath>
    <AppendRuntimeIdentifierToOutputPath>false</AppendRuntimeIdentifierToOutputPath>
    <PlatformTarget>x64</PlatformTarget>
    <IsPublishable>False</IsPublishable>
    <AssemblyName>$(MSBuildProjectName).CSharp</AssemblyName>
  </PropertyGroup>

  <ItemGroup>
    <ProjectReference Include="F:\CPP.REF\Engine\Source\Runtime\CoreAObject.CSharp\CoreAObject.csproj" />
  </ItemGroup>

  <ItemGroup>
    <Using Include="AE.CoreAObject.Object">
      <Alias>Object</Alias>
    </Using>
  </ItemGroup>

</Project>
)");

		co_await File::CompareAndWriteAllTextAsync(
			Path::Combine(CSharpPath, String::Format(TEXT("{0}.csproj"), PackageName)),
			String::Format(CSharpProject, PackageName),
			InCancellationToken
		);
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
	Output.WriteLine(TEXT("  -CSharp [String] The include path to save generated CSharp codes."));
	Output.WriteLine(TEXT("  -PackageName [String] The package name for scripts."));
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

	if (CommandLine::TryGetValue(TEXT("csharp"), CSharpPath) == false || CSharpPath.IsEmpty())
	{
		throw TerminateException(TerminateException::EKnownErrorCodes::Usage);
	}

	if (CommandLine::TryGetValue(TEXT("packagename"), PackageName) == false || PackageName.IsEmpty())
	{
		throw TerminateException(TerminateException::EKnownErrorCodes::Usage);
	}
}