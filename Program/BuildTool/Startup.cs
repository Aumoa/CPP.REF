// Copyright 2020-2022 Aumoa.lib. All right reserved.

using BuildTool;

using DotNETUtilities;

if (!args.IsValidIndex(0))
{
    Log.Error(null, "Argument error. Specify *.Solution.cs to compile your solution metadata.");
    return -1;
}

FileReference solutionCodeFile = new(args[0]);
Solution solution = new(solutionCodeFile);

ISolutionGenerator[] generators = new[]
{
    new VSGenerator(solution, VisualStudioVersion.VS2022)
};

foreach (var generator in generators)
{
    generator.Generate();
}

Log.Shutdown();
return 0;