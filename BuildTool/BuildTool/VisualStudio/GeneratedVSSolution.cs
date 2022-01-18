// Copyright 2020-2021 Aumoa.lib. All right reserved.

using DotNETUtilities;

using System.Text;

namespace BuildTool;

class GeneratedVSSolution : IGeneratedSolution
{
    public readonly Solution CompiledSolution;

    public GeneratedVSSolution(Solution CompiledSolution)
    {
        this.CompiledSolution = CompiledSolution;
    }

    Dictionary<string, GeneratedVSProject> GeneratedProjects;

    struct SolutionDirectoryPair
    {
        public string RecursivePath;
        public string Guid;
    }

    public void Generate(VisualStudioGenerator SlnGenerator)
    {
        GeneratedProjects = SlnGenerator.GeneratedProjects;

        StringBuilder Builder = new();

        const string CppProjectGuid = "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942";
        const string SolutionDirectoryGuid = "2150E333-8FDC-42A3-9474-1A3956D46DE8";

        Builder.AppendLine();
        Builder.AppendLine("Microsoft Visual Studio Solution File, Format Version 12.00");
        Builder.AppendLine("# Visual Studio Version 16");
        Builder.AppendLine("VisualStduioVersion = 16.0.31321.278");
        Builder.AppendLine("MinimumVisualStudioVersion = 10.0.40219.1");

        Dictionary<string, string> CachedSlicedSolutionDirectory = new();

        Dictionary<string, SolutionDirectoryPair> SolutionDirectories = new();
        foreach (var (Key, Value) in GeneratedProjects)
        {
            var Rule = Value.CompiledProject.CompiledRule;
            string[] RecursivePathSlice = Rule.RelativePath.Split(".", StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);

            string RecursivePath = "";
            foreach (var Slice in RecursivePathSlice)
            {
                RecursivePath += Slice;
                SolutionDirectories.TryAdd(Slice, new SolutionDirectoryPair
                {
                    RecursivePath = RecursivePath,
                    Guid = this.GenerateGuid(RecursivePath).ToString()
                });

                RecursivePath += ".";
            }

            CachedSlicedSolutionDirectory[Key] = RecursivePathSlice[^1];
        }

        foreach (var (Key, Value) in SolutionDirectories)
        {
            Builder.AppendLine(string.Format("Project(\"{{{0}}}\") = \"{1}\", \"{1}\", \"{{{2}}}\"", SolutionDirectoryGuid, Key, Value.Guid));
            Builder.AppendLine("EndProject");
        }

        // Generate C++ projects.
        List<string> ProjectsSort = new();
        foreach (var (Key, Value) in GeneratedProjects)
        {
            string ProjectDeclare = string.Format("Project(\"{{{0}}}\") = \"{1}\", \"{2}\", \"{{{3}}}\"\nEndProject", CppProjectGuid, Key, Value.ProjectFile, Value.GeneratedGuid);

            if (Value.CompiledProject == SlnGenerator.PrimaryModule)
            {
                ProjectsSort.Insert(0, ProjectDeclare);
            }
            else
            {
                ProjectsSort.Add(ProjectDeclare);
            }
        }

        foreach (var Declare in ProjectsSort)
        {
            Builder.AppendLine(Declare);
        }

        string[] Configurations = new[] { "Debug", "DebugGame", "Release" };
        Builder.AppendLine("Global");
        {
            // SolutionConfigurationPlatforms
            Builder.AppendLine("\tGlobalSection(SolutionConfigurationPlatforms) = preSolution");
            {
                foreach (var Config in Configurations)
                {
                    Builder.AppendLine(string.Format("\t\t{0}|x64 = {0}|x64", Config));
                }
            }
            Builder.AppendLine("\tEndGlobalSection");

            // ProjectConfigurationPlatforms
            Builder.AppendLine("\tGlobalSection(ProjectConfigurationPlatforms) = postSolution");
            {
                foreach (var (Key, Project) in GeneratedProjects)
                {
                    foreach (var Config in Configurations)
                    {
                        Builder.AppendLine(string.Format("\t\t{{{0}}}.{1}|x64.ActiveCfg = {1}|x64", Project.GeneratedGuid, Config));
                        Builder.AppendLine(string.Format("\t\t{{{0}}}.{1}|x64.Build.0 = {1}|x64", Project.GeneratedGuid, Config));
                    }
                }
            }
            Builder.AppendLine("\tEndGlobalSection");

            // SolutionProperties
            Builder.AppendLine("\tGlobalSection(SolutionProperties) = preSolution");
            {
                Builder.AppendLine("\t\tHideSolutionNode = FALSE");
            }
            Builder.AppendLine("\tEndGlobalSection");

            // NestedProjects
            Builder.AppendLine("\tGlobalSection(NestedProjects) = preSolution");
            {
                foreach (var (Key, Value) in SolutionDirectories)
                {
                    string[] Splits = Value.RecursivePath.Split(".");
                    if (Splits.Length > 1)
                    {
                        string LastParent = Splits[^2];
                        SolutionDirectoryPair LastParentInfo = SolutionDirectories[LastParent];
                        Builder.AppendLine(string.Format("\t\t{{{0}}} = {{{1}}}", Value.Guid, LastParentInfo.Guid));
                    }
                }

                foreach (var (Key, Project) in GeneratedProjects)
                {
                    SolutionDirectoryPair ParentInfo = SolutionDirectories[CachedSlicedSolutionDirectory[Key]];
                    Builder.AppendLine(string.Format("\t\t{{{0}}} = {{{1}}}", Project.GeneratedGuid, ParentInfo.Guid));
                }
            }
            Builder.AppendLine("\tEndGlobalSection");

            // ExtensibilityGlobals
            Builder.AppendLine("\tGlobalSection(ExtensibilityGlobals) = postSolution");
            {
                Builder.AppendLine($"\t\tSolutionGuid = {{{CompiledSolution.GenerateGuid(CompiledSolution.CompiledRule.Name)}}}");
            }
            Builder.AppendLine("\tEndGlobalSection");
        }
        Builder.AppendLine("EndGlobal");

        SolutionFile = new FileReference($"{CompiledSolution.CompiledRule.Name}.sln");
        SolutionFileData = Builder;
    }

    public void SaveAll()
    {
        foreach (var (_, VSProject) in GeneratedProjects)
        {
            VSProject.SaveAll();
        }

        string CurrentFile = SolutionFileData.ToString();
        if (!SolutionFile.IsExist || SolutionFile.ReadAllText() != CurrentFile)
        {
            File.WriteAllText(SolutionFile.FullPath, CurrentFile);
        }
    }

    public virtual FileReference SolutionFile { get; private set; }
    private StringBuilder SolutionFileData;
}