// Copyright 2020-2021 Aumoa.lib. All right reserved.

namespace BuildTool;

class VisualStudioGenerator : ISolutionGenerator
{
    public readonly VisualStudioVersion VSVersion;

    public VisualStudioGenerator(VisualStudioVersion VSVersion)
    {
        this.VSVersion = VSVersion;
    }

    public readonly Dictionary<string, GeneratedVSProject> GeneratedProjects = new();
    public Solution Solution { get; private set; }
    public Project PrimaryModule { get; private set; }

    public virtual IGeneratedSolution Generate(Solution CompiledSolution)
    {
        Solution = CompiledSolution;

        foreach (var Project in CompiledSolution.AllProjects)
        {
            var GeneratedProject = new GeneratedVSProject(Project);
            GeneratedProjects.Add(GeneratedProject.CompiledProject.CompiledRule.Name, GeneratedProject);

            if (Project.CompiledRule.Name == CompiledSolution.CompiledRule.PrimaryModule)
            {
                PrimaryModule = Project;
            }
        }

        if (PrimaryModule == null)
        {
            throw new SolutionException("PrimaryModule is not specified in solution rule code.");
        }

        foreach (var (_, GeneratedProject) in GeneratedProjects)
        {
            GeneratedProject.Generate(this);
        }

        GeneratedVSSolution VSSolution = new(CompiledSolution);
        VSSolution.Generate(this);

        return VSSolution;
    }
}