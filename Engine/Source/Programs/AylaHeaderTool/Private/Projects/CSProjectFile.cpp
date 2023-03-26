// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Projects/CSProjectFile.h"

CSProjectFile::CSProjectFile(String InPath, String AssemblyName)
	: ProjectPath(InPath)
	, AssemblyName(AssemblyName)
{
}

Task<> CSProjectFile::GenerateProjectFileAsync(String BuildOutputPath, std::stop_token SToken)
{
    String DirectoryName = Path::Combine(ProjectPath, AssemblyName);
	String ProjectFile = Path::Combine(DirectoryName, Path::ChangeExtension(AssemblyName, TEXT(".csproj")));

	String SourceCode = TEXT(R"ABC(
<Project Sdk="Microsoft.NET.Sdk">

  <PropertyGroup>
    <TargetFramework>net6.0</TargetFramework>
    <ImplicitUsings>enable</ImplicitUsings>
    <Nullable>enable</Nullable>
    <AppendTargetFrameworkToOutputPath>false</AppendTargetFrameworkToOutputPath>
    <OutputPath>{0}</OutputPath>
    <AssemblyName>{1}.Interop</AssemblyName>
    <RootNamespace>{1}</RootNamespace>
  </PropertyGroup>

</Project>
)ABC");

    SourceCode = String::Format(SourceCode, BuildOutputPath, AssemblyName);
    Directory::CreateDirectory(DirectoryName);
    co_await File::WriteAllTextAsync(ProjectFile, SourceCode, 0, SToken);
}