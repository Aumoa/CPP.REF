// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Text.RegularExpressions;

namespace AylaEngine;

internal class DotNETCompiler
{
    private static SemaphoreSlim m_Access = new(1);

    private static IEnumerable<string> GatherSourceCodes(string projectFile, GroupDescriptor group, TargetInfo targetInfo)
    {
        var projectDir = Path.GetDirectoryName(projectFile)!;
        var allSourceFiles = Directory.GetFiles(projectDir, "*.*", SearchOption.AllDirectories);
        var fileName = Path.GetFileNameWithoutExtension(projectFile);
        string intDir = group.Intermediate(fileName, targetInfo, FolderPolicy.PathType.Current);
        var outputFile = group.OutputFileName(targetInfo, fileName, ModuleType.Library, FolderPolicy.PathType.Current);
        var objDir = Path.GetFullPath(Path.Combine(projectDir, "obj"));
        var binDir = Path.GetFullPath(Path.Combine(projectDir, "bin"));

        foreach (var sourceFile in allSourceFiles)
        {
            if (sourceFile.StartsWith(objDir))
            {
                continue;
            }

            if (sourceFile.StartsWith(binDir))
            {
                continue;
            }

            var ext = Path.GetExtension(sourceFile);
            if (ext.Equals(".cs", StringComparison.OrdinalIgnoreCase))
            {
                yield return sourceFile;
            }
        }
    }

    private static void GenerateCache(string projectFile, GroupDescriptor group, TargetInfo targetInfo)
    {
        var projectDir = Path.GetDirectoryName(projectFile);
        if (projectDir == null)
        {
            return;
        }

        var fileName = Path.GetFileNameWithoutExtension(projectFile);
        string intDir = group.Intermediate(fileName, targetInfo, FolderPolicy.PathType.Current);

        if (Directory.Exists(intDir))
        {
            Directory.Delete(intDir, true);
        }

        Directory.CreateDirectory(intDir);

        foreach (var sourceFile in GatherSourceCodes(projectFile, group, targetInfo))
        {
            var relativeFileName = Path.GetRelativePath(projectDir, sourceFile);
            var fileId = relativeFileName.Replace(Path.DirectorySeparatorChar, '_');
            var cacheFileName = Path.Combine(intDir, fileId + ".cache");
            var current = SourceCodeCache.MakeCachedSimple(sourceFile, null);
            current.SaveCached(cacheFileName);
        }
    }

    public static bool NeedCompile(string projectFile, GroupDescriptor group, TargetInfo targetInfo)
    {
        var projectDir = Path.GetDirectoryName(projectFile);
        if (projectDir == null)
        {
            return true;
        }

        var fileName = Path.GetFileNameWithoutExtension(projectFile);
        string intDir = group.Intermediate(fileName, targetInfo, FolderPolicy.PathType.Current);
        var outputFile = group.OutputFileName(targetInfo, fileName, ModuleType.Library, FolderPolicy.PathType.Current);
        if (File.Exists(outputFile) == false)
        {
            return true;
        }

        if (Directory.Exists(intDir) == false)
        {
            return true;
        }

        HashSet<string> cacheFiles = [];
        cacheFiles.AddRange(Directory.GetFiles(intDir, "*.cache", SearchOption.TopDirectoryOnly));

        foreach (var sourceFile in GatherSourceCodes(projectFile, group, targetInfo))
        {
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

            cacheFiles.Remove(cacheFileName);
        }

        return cacheFiles.Count > 0;
    }

    public async Task<string> CompileAsync(string projectFile, GroupDescriptor group, TargetInfo targetInfo, CancellationToken cancellationToken = default)
    {
        await m_Access.WaitAsync(cancellationToken);
        try
        {
            var sourceDirectory = Path.GetDirectoryName(projectFile);
            if (sourceDirectory == null)
            {
                throw new InvalidOperationException();
            }

            var sourceFiles = GatherSourceCodes(projectFile, group, targetInfo)
                .Select(CSCompiler.SourceCodeProvider.FromFile);

            var config = VSUtility.GetConfigName(targetInfo);
            var platform = VSUtility.GetArchitectureName(targetInfo);

            CSharpProject.Parse(File.ReadAllText(projectFile));

            // This is a simple example of project parsing, which should later be replaced with formal parsing.
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

            GenerateCache(projectFile, group, targetInfo);
            return outputDll;
        }
        catch (CSCompilerError error)
        {
            Console.Error.WriteLine(error.Message);
            throw TerminateException.User();
        }
        finally
        {
            m_Access.Release();
        }
    }
}
