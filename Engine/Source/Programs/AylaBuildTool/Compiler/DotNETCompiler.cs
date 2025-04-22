// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Reflection;
using System.Text.RegularExpressions;

namespace AylaEngine;

internal class DotNETCompiler
{
    private static SemaphoreSlim m_Access = new(1);

    private static IEnumerable<string> GatherSourceCodes(string sourceDirectory, string assemblyName, GroupDescriptor group, TargetInfo targetInfo)
    {
        var allSourceFiles = Directory.GetFiles(sourceDirectory, "*.*", SearchOption.AllDirectories);
        string intDir = group.Intermediate(assemblyName, targetInfo, FolderPolicy.PathType.Current);
        var outputFile = group.OutputFileName(targetInfo, assemblyName, ModuleType.Library, FolderPolicy.PathType.Current);
        var objDir = Path.GetFullPath(Path.Combine(sourceDirectory, "obj"));
        var binDir = Path.GetFullPath(Path.Combine(sourceDirectory, "bin"));

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

    private static void GenerateCache(string sourceDirectory, string assemblyName, GroupDescriptor group, TargetInfo targetInfo)
    {
        string intDir = group.Intermediate(assemblyName, targetInfo, FolderPolicy.PathType.Current);

        if (Directory.Exists(intDir))
        {
            Directory.Delete(intDir, true);
        }

        Directory.CreateDirectory(intDir);

        foreach (var sourceFile in GatherSourceCodes(sourceDirectory, assemblyName, group, targetInfo))
        {
            var relativeFileName = Path.GetRelativePath(sourceDirectory, sourceFile);
            var fileId = relativeFileName.Replace(Path.DirectorySeparatorChar, '_');
            var cacheFileName = Path.Combine(intDir, fileId + ".cache");
            var current = SourceCodeCache.MakeCachedSimple(sourceFile, null);
            current.SaveCached(cacheFileName);
        }
    }

    public static bool NeedCompile(string projectFile, GroupDescriptor group, TargetInfo targetInfo)
    {
        string? sourceDirectory = Path.GetDirectoryName(projectFile);
        if (sourceDirectory == null)
        {
            return true;
        }

        string assemblyName = Path.GetFileNameWithoutExtension(projectFile);
        return NeedCompile(sourceDirectory, assemblyName, group, targetInfo);
    }

    public static bool NeedCompile(string sourceDirectory, string assemblyName, GroupDescriptor group, TargetInfo targetInfo)
    {
        string intDir = group.Intermediate(assemblyName, targetInfo, FolderPolicy.PathType.Current);
        var outputFile = group.OutputFileName(targetInfo, assemblyName, ModuleType.Library, FolderPolicy.PathType.Current);
        var objDir = Path.GetFullPath(Path.Combine(sourceDirectory, "obj"));
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

        foreach (var sourceFile in GatherSourceCodes(sourceDirectory, assemblyName, group, targetInfo))
        {
            var relativeFileName = Path.GetRelativePath(sourceDirectory, sourceFile);
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

    public async Task<string> CompileAsync(string sourceDirectory, string assemblyName, string projectDescription, GroupDescriptor group, TargetInfo targetInfo, CancellationToken cancellationToken = default)
    {
        await m_Access.WaitAsync(cancellationToken);
        try
        {
            var sourceFiles = GatherSourceCodes(sourceDirectory, assemblyName, group, targetInfo)
                .Select(CSCompiler.SourceCodeProvider.FromFile);

            var config = VSUtility.GetConfigName(targetInfo);
            var platform = VSUtility.GetArchitectureName(targetInfo);

            sourceFiles = sourceFiles
                .Append(CSharpProject.GenerateAssemblyAttribute(new Version(9, 0)))
                .Append(CSharpProject.GenerateAssemblyInfo(null, config, null, assemblyName, new Version(1, 0, 0, 0)));

            var condition = CSharpProject.Condition.Parse($"$(Configuration)|$(Platform)=='{config}|{platform}'");
            var csproj = CSharpProject.Parse(projectDescription).Freeze(condition);

            List<string> referencedAssemblies = [];
            foreach (var referenceBase in csproj.ItemGroup.References ?? [])
            {
                referencedAssemblies.Add(referenceBase.ReferencedAssemblyPath(csproj.ItemGroup.Condition, sourceDirectory));
            }

            var outputDir = group.Output(targetInfo, FolderPolicy.PathType.Current);
            var outputDll = Path.Combine(outputDir, assemblyName + ".dll");
            await CSCompiler.CompileToAsync(assemblyName, outputDll, sourceFiles, CSCompiler.GetDefaultAssemblies().Concat(referencedAssemblies), cancellationToken);

            GenerateCache(sourceDirectory, assemblyName, group, targetInfo);
            return outputDll;
        }
        catch (CSCompilerError error)
        {
            Console.Error.WriteLine("{0}: {1}", assemblyName, error.Message);
            throw TerminateException.User();
        }
        catch (Exception error)
        {
            Console.Error.WriteLine("{0}: {1}", assemblyName, error);
            throw TerminateException.Internal();
        }
        finally
        {
            m_Access.Release();
        }
    }

    public async Task<string> CompileAsync(string projectFile, GroupDescriptor group, TargetInfo targetInfo, CancellationToken cancellationToken = default)
    {
        var sourceDirectory = Path.GetDirectoryName(projectFile);
        if (sourceDirectory == null)
        {
            throw new InvalidOperationException();
        }

        var assemblyName = Path.GetFileNameWithoutExtension(projectFile);
        string csproj = await File.ReadAllTextAsync(projectFile);
        return await CompileAsync(sourceDirectory, assemblyName, csproj, group, targetInfo, cancellationToken);
    }
}
