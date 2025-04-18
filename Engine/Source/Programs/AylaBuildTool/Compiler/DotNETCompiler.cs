// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Text.RegularExpressions;
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
        var objDir = Path.GetFullPath(Path.Combine(projectDir, "obj"));

        foreach (var sourceFile in allSourceFiles)
        {
            if (sourceFile.StartsWith(objDir))
            {
                continue;
            }

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

    public async Task CompileAsync(string projectFile, GroupDescriptor group, TargetInfo targetInfo, CancellationToken cancellationToken = default)
    {
        await m_Access.WaitAsync(cancellationToken);
        try
        {
            var sourceDirectory = Path.GetDirectoryName(projectFile);
            if (sourceDirectory == null)
            {
                throw new InvalidOperationException();
            }

            var sourceFiles = Directory
                .GetFiles(sourceDirectory, "*.cs", SearchOption.AllDirectories)
                .Select(CSCompiler.SourceCodeProvider.FromFile);

            var config = VSUtility.GetConfigName(targetInfo);
            var platform = VSUtility.GetArchitectureName(targetInfo);

            CSharpProject.Parse(File.ReadAllText(projectFile));

            // This is a simple example of project parsing, which should later be replaced with formal parsing.
            Console.WriteLine(projectFile);
            var csproj = await File.ReadAllTextAsync(projectFile, cancellationToken);
            var csprojLines = csproj.Split(["\n", "\r\n"], StringSplitOptions.None);
            var itemGroupIndex = Array.FindIndex(csprojLines, p => p.Contains($"<ItemGroup Condition=\"'$(Configuration)|$(Platform)'=='{config}|{platform}'\">"));
            List<string> referencedAssemblies = [];
            if (itemGroupIndex != -1)
            {
                var endIndex = Array.FindIndex(csprojLines, itemGroupIndex, p => p.Contains("</ItemGroup>"));
                var imports = csprojLines[itemGroupIndex..endIndex];
                foreach (var line in imports)
                {
                    var match = Regex.Match(line, @"<HintPath>(.*?)<\/HintPath>");
                    if (match.Success)
                    {
                        referencedAssemblies.Add(match.Groups[1].Value);
                    }

                    match = Regex.Match(line, @"<ProjectReference Include=""(.*?)"" \/>");
                    if (match.Success)
                    {
                        if (match.Success)
                        {
                            var referencedProject = match.Groups[1].Value;
                            var referencedAssemblyName = Path.GetFileNameWithoutExtension(referencedProject);
                            var referencedAssembly = group.OutputFileName(targetInfo, referencedAssemblyName, ModuleType.Library, FolderPolicy.PathType.Current);
                            referencedAssemblies.Add(referencedAssembly);
                        }
                    }
                }
            }

            var outputDir = group.Output(targetInfo, FolderPolicy.PathType.Current);
            var assemblyName = Path.GetFileNameWithoutExtension(projectFile);
            var outputDll = Path.Combine(outputDir, assemblyName + ".dll");
            await CSCompiler.CompileToAsync(assemblyName, outputDll, sourceFiles, CSCompiler.GetDefaultAssemblies().Concat(referencedAssemblies), cancellationToken);

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
        finally
        {
            m_Access.Release();
        }
    }
}
