using System.Text.Encodings.Web;
using System.Text.Json;

namespace AylaEngine;

internal class VSCSolutionGenerator : Generator
{
    private static readonly JsonSerializerOptions JsonOptions = new()
    {
        Encoder = JavaScriptEncoder.UnsafeRelaxedJsonEscaping,
        WriteIndented = true
    };

    /// <summary>
    /// Asynchronously generates Visual Studio Code workspace and project files for the specified solution.
    /// </summary>
    /// <param name="solution">The solution object containing the projects to generate files for.</param>
    /// <param name="cancellationToken">A token to monitor for cancellation requests.</param>
    /// <returns>A <see cref="ValueTask"/> representing the asynchronous operation.</returns>
    public override async ValueTask GenerateAsync(Solution solution, CancellationToken cancellationToken = default)
    {
        List<Task> tasks = [];
        List<string> outputFolders = [];
        foreach (var project in solution.Projects)
        {
            if (project is ModuleProject mp)
            {
                tasks.Add(VSCCppProjectGenerator.GenerateAsync(solution, mp, outputFolders, cancellationToken).AsTask());
            }
            else if (project is ProgramProject pp)
            {
                outputFolders.Add(Path.GetDirectoryName(pp.ProjectFilePath)!);
            }
        }

        await Task.WhenAll(tasks);

        var workspaceFileName = Path.Combine(solution.PrimaryGroup.RootDirectory, solution.PrimaryGroup.Name + ".code-workspace");

        var workspace = new
        {
            folders = outputFolders.Select(p => new { path = p }),
            settings = new Dictionary<string, object>
            {
                ["C_Cpp.default.configurationProvider"] = "ms-vscode.cmake-tools",
                ["files.exclude"] = new Dictionary<string, bool>
                {
                    { "**/Intermediate", true },
                    { "**/Binaries", true }
                }
            }
        };

        var json = JsonSerializer.Serialize(workspace, JsonOptions);
        await File.WriteAllTextAsync(workspaceFileName, json, cancellationToken);
    }
}
