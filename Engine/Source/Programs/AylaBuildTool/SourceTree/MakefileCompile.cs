// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Compile;
using AE.Misc;

namespace AE.SourceTree;

public sealed record MakefileCompile
{
    public required string SourceCode { get; init; }

    public required MakefileCache Cache { get; init; }

    public required CppModuleDescriptor? Provide { get; init; }

    public required CppModuleDescriptor[] Requires { get; init; }

    public required string ModuleName { get; init; }

    public required string[] DependModules { get; init; }

    public required string[] IncludePaths { get; init; }

    public required string[] AdditionalMacros { get; init; }

    public required int[] DisableWarnings { get; init; }
}
