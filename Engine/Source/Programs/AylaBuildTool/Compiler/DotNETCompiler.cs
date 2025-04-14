// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using Spectre.Console;

namespace AylaEngine;

internal class DotNETCompiler
{
    private static SemaphoreSlim m_Access = new(1);

    private static void GenerateCache(string projectFile, GroupDescriptor group, TargetInfo targetInfo)
    {
        var projectDir = Path.GetDirectoryName(projectFile);
        if (projectDir == null)
        {
            return;
        }

        var allSourceFiles = Directory.GetFiles(projectDir, "*.*", SearchOption.AllDirectories);
        var fileName = Path.GetFileNameWithoutExtension(projectFile);
        string intDir = group.Intermediate(fileName, targetInfo, FolderPolicy.PathType.Current);
        var outputFile = group.OutputFileName(targetInfo, fileName, ModuleType.Library, FolderPolicy.PathType.Current);

        foreach (var sourceFile in allSourceFiles)
        {
            var ext = Path.GetExtension(sourceFile);
            if (ext.Equals(".csproj", StringComparison.OrdinalIgnoreCase) == false && ext.Equals(".cs", StringComparison.OrdinalIgnoreCase) == false)
            {
                continue;
            }

            var relativeFileName = Path.GetRelativePath(projectDir, sourceFile);
            var fileId = relativeFileName.Replace(Path.DirectorySeparatorChar, '_');
            var cacheFileName = Path.Combine(intDir, fileId + ".cache");
            var current = SourceCodeCache.MakeCachedSimple(sourceFile, null);
            current.SaveCached(cacheFileName);
        }
    }

    public static bool NeedCompile(string projectFile, GroupDescriptor group, TargetInfo targetInfo)
    {
        if (File.Exists(projectFile) == false)
        {
            return true;
        }

        var projectDir = Path.GetDirectoryName(projectFile);
        if (projectDir == null)
        {
            return true;
        }

        var allSourceFiles = Directory.GetFiles(projectDir, "*.*", SearchOption.AllDirectories);
        var fileName = Path.GetFileNameWithoutExtension(projectFile);
        string intDir = group.Intermediate(fileName, targetInfo, FolderPolicy.PathType.Current);
        var outputFile = group.OutputFileName(targetInfo, fileName, ModuleType.Library, FolderPolicy.PathType.Current);
        if (File.Exists(outputFile) == false)
        {
            return true;
        }

        foreach (var sourceFile in allSourceFiles)
        {
            var ext = Path.GetExtension(sourceFile);
            if (ext.Equals(".csproj", StringComparison.OrdinalIgnoreCase) == false && ext.Equals(".cs", StringComparison.OrdinalIgnoreCase) == false)
            {
                continue;
            }

            var relativeFileName = Path.GetRelativePath(projectDir, sourceFile);
            var fileId = relativeFileName.Replace(Path.DirectorySeparatorChar, '_');
            var cacheFileName = Path.Combine(intDir, fileId + ".cache");
            if (File.Exists(cacheFileName) == false)
            {
                return true;
            }

            var current = SourceCodeCache.MakeCachedSimple(sourceFile, null);
            var cache = SourceCodeCache.LoadCached(cacheFileName);
            if (cache.IsModified(current))
            {
                return true;
            }
        }

        return false;
    }

    public async Task<Terminal.Output> CompileAsync(string projectFile, GroupDescriptor group, TargetInfo targetInfo, CancellationToken cancellationToken = default)
    {
        await m_Access.WaitAsync(cancellationToken);
        try
        {
            var config = VSUtility.GetConfigName(targetInfo);
            var platform = VSUtility.GetArchitectureName(targetInfo);
            var output = await Terminal.ExecuteCommandAsync($"build {projectFile} -c \"{config}\" /p:Platform=\"{platform}\" --nologo", new Terminal.Options
            {
                Executable = "dotnet",
                Logging = Terminal.Logging.None,
                WorkingDirectory = Path.GetDirectoryName(projectFile) ?? string.Empty,
            }, cancellationToken);

            if (output.IsCompletedSuccessfully)
            {
                var outputDir = group.Output(targetInfo, FolderPolicy.PathType.Current);
                var outputDll = Path.Combine(outputDir, Path.GetFileNameWithoutExtension(projectFile) + ".dll");
                if (ConsoleEnvironment.IsDynamic)
                {
                    AnsiConsole.MarkupLine("[green]{0} generated.[/]", outputDll);
                }
                else
                {
                    Console.WriteLine("{0} generated.", outputDll);
                }

                GenerateCache(projectFile, group, targetInfo);
            }
            else
            {
                if (ConsoleEnvironment.IsDynamic)
                {
                    AnsiConsole.MarkupLine("[red]{0}[/]", string.Join('\n', output.Logs.Select(p => p.Value)).EscapeMarkup());
                }
                else
                {
                    Console.WriteLine(string.Join('\n', output.Logs.Select(p => p.Value)));
                }
            }

            return output;
        }
        finally
        {
            m_Access.Release();
        }
    }
}
