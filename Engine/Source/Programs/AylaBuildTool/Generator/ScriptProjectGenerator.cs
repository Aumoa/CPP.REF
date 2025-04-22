// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine;

// The Script project is not dependent on Visual Studio.
internal static class ScriptProjectGenerator
{
    public static async Task GenerateAsync(Solution solution, Dictionary<ModuleProject, string> scriptProjectPaths, ModuleProject project, CancellationToken cancellationToken)
    {
        var fileName = project.GetScriptProjectFileName();

        var platforms = string.Join(';', TargetInfo.GetAllTargets()
            .Select(p => VSUtility.GetArchitectureName(p))
            .Distinct());

        var configurations = string.Join(';', TargetInfo.GetAllTargets()
            .Select(p => VSUtility.GetCppConfigName(p))
            .Distinct());

        if (TargetInfo.GetAllTargets().Any(p => project.GetRule(p).EnableScript) == false)
        {
            return;
        }

        var csproj = CSGenerator.GenerateModule(solution, project, true, TargetInfo.GetAllTargets());
        await TextFileHelper.WriteIfChangedAsync(fileName, csproj, cancellationToken);

        lock (scriptProjectPaths)
        {
            scriptProjectPaths.Add(project, fileName);
        }
    }
}
