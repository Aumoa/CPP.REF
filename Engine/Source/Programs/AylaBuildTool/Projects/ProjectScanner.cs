namespace AylaEngine;

internal sealed class ProjectScanner
{
    public async Task<IReadOnlyList<ProjectCandidate>> ScanAsync(
        GroupDescriptor group,
        string sourceDirectory,
        CancellationToken cancellationToken)
    {
        return await ScanDirectoryRecursiveAsync(group, sourceDirectory, cancellationToken);
    }

    private async Task<ProjectCandidate[]> ScanDirectoryRecursiveAsync(
        GroupDescriptor group,
        string currentDirectory,
        CancellationToken cancellationToken)
    {
        cancellationToken.ThrowIfCancellationRequested();

        var projectName = Path.GetFileName(currentDirectory);
        var projectFileName = Path.Combine(currentDirectory, projectName + ".csproj");
        if (File.Exists(projectFileName))
        {
            return
            [
                new ProjectCandidate(
                    ProjectCandidateKind.Program,
                    projectName,
                    group,
                    currentDirectory,
                    projectFileName)
            ];
        }

        var ruleFileName = Path.Combine(currentDirectory, projectName + ".Module.cs");
        if (File.Exists(ruleFileName))
        {
            return
            [
                new ProjectCandidate(
                    ProjectCandidateKind.Module,
                    projectName,
                    group,
                    currentDirectory,
                    ruleFileName)
            ];
        }

        var tasks = Directory.GetDirectories(currentDirectory, "*", SearchOption.TopDirectoryOnly)
            .Select(subDirectory => ScanDirectoryRecursiveAsync(group, subDirectory, cancellationToken));
        var results = await Task.WhenAll(tasks);
        return results.SelectMany(projects => projects).ToArray();
    }
}
