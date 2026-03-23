// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Text;

namespace AylaEngine;

internal class VSSlnxSolutionGenerator : Generator
{
    public override async ValueTask GenerateAsync(Solution solution, CancellationToken cancellationToken = default)
    {
        var solutionDir = solution.PrimaryGroup.RootDirectory;
        var solutionFileName = Path.Combine(solutionDir, solution.PrimaryGroup.Name + ".slnx");

        Dictionary<ModuleProject, string> vcxprojPaths = new();
        List<Task> tasks = [];
        foreach (var project in solution.Projects.OfType<ModuleProject>())
        {
            tasks.Add(VSCppProjectGenerator.GenerateAsync(solution, vcxprojPaths, project, cancellationToken));
            if (project.IsScriptable())
            {
                tasks.Add(VSScriptProjectGenerator.GenerateAsync(solution, project, cancellationToken).AsTask());
            }
        }

        await Task.WhenAll(tasks);

        // Categorize projects for folder structure
        var engineRuntime = new List<ModuleProject>();
        var engineEditor = new List<ModuleProject>();
        var engineThirdParty = new List<ModuleProject>();
        var engineProgram = new List<ModuleProject>();
        var programs = new List<ProgramProject>();
        var game = new List<ModuleProject>();

        foreach (var project in solution.Projects)
        {
            if (project is ProgramProject pp)
            {
                programs.Add(pp);
            }
            else if (project is ModuleProject mp)
            {
                var directoryName = mp.SourceDirectory.Replace(mp.Group.RootDirectory, string.Empty);
                var normalizedDir = directoryName.Replace('\\', '/');
                if (project.Group.Name == "Engine")
                {
                    if (normalizedDir.Contains("/Editor/"))
                        engineEditor.Add(mp);
                    else if (normalizedDir.Contains("/ThirdParty/"))
                        engineThirdParty.Add(mp);
                    else if (normalizedDir.Contains("/Programs/"))
                        engineProgram.Add(mp);
                    else
                        engineRuntime.Add(mp);
                }
                else
                {
                    game.Add(mp);
                }
            }
        }

        var builder = new StringBuilder();
        builder.Append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
        builder.Append("<Solution>\n");

        // Engine folder with Runtime and Editor subfolders
        builder.Append("  <Folder Name=\"Engine\">\n");

        builder.Append("    <Folder Name=\"Runtime\">\n");
        foreach (var p in engineRuntime)
        {
            WriteModuleProject(builder, solutionDir, vcxprojPaths, p, 6);
        }
        builder.Append("    </Folder>\n");

        if (engineEditor.Count > 0)
        {
            builder.Append("    <Folder Name=\"Editor\">\n");
            foreach (var p in engineEditor)
            {
                WriteModuleProject(builder, solutionDir, vcxprojPaths, p, 6);
            }
            builder.Append("    </Folder>\n");
        }

        builder.Append("  </Folder>\n");

        // ThirdParty folder
        if (engineThirdParty.Count > 0)
        {
            builder.Append("  <Folder Name=\"ThirdParty\">\n");
            foreach (var p in engineThirdParty)
            {
                WriteModuleProject(builder, solutionDir, vcxprojPaths, p, 4);
            }
            builder.Append("  </Folder>\n");
        }

        // Program folder
        if (programs.Count > 0 || engineProgram.Count > 0)
        {
            builder.Append("  <Folder Name=\"Program\">\n");
            foreach (var pp in programs)
            {
                var relativePath = Path.GetRelativePath(solutionDir, pp.ProjectFilePath);
                builder.Append($"    <Project Path=\"{relativePath}\" />\n");
            }
            foreach (var mp in engineProgram)
            {
                WriteModuleProject(builder, solutionDir, vcxprojPaths, mp, 4);
            }
            builder.Append("  </Folder>\n");
        }

        // Game folder (only if non-engine projects exist)
        if (game.Count > 0)
        {
            builder.Append("  <Folder Name=\"Game\">\n");
            foreach (var p in game)
            {
                WriteModuleProject(builder, solutionDir, vcxprojPaths, p, 4);
            }
            builder.Append("  </Folder>\n");
        }

        builder.Append("</Solution>\n");

        await TextFileHelper.WriteIfChangedAsync(solutionFileName, builder.ToString(), cancellationToken);
    }

    private static void WriteModuleProject(StringBuilder builder, string solutionDir, Dictionary<ModuleProject, string> vcxprojPaths, ModuleProject project, int indentSpaces)
    {
        var indent = new string(' ', indentSpaces);
        string vcxprojPath;
        lock (vcxprojPaths)
        {
            vcxprojPath = vcxprojPaths[project];
        }

        var relativePath = Path.GetRelativePath(solutionDir, vcxprojPath);
        builder.Append($"{indent}<Project Path=\"{relativePath}\" />\n");

        if (project.IsScriptable())
        {
            var scriptRelativePath = Path.GetRelativePath(solutionDir, project.ScriptProjectFileName);
            builder.Append($"{indent}<Project Path=\"{scriptRelativePath}\" />\n");
        }
    }
}
