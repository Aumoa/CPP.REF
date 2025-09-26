// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Reflection;
using System.Text.RegularExpressions;
using System.Xml;

namespace AylaEngine;

internal class DotNETCompiler
{
    private static SemaphoreSlim m_Access = new(1);

    private static IEnumerable<string> GatherSourceCodes(Installation installation, string sourceDirectory, string assemblyName, GroupDescriptor group, TargetInfo targetInfo)
    {
        var allSourceFiles = Directory.GetFiles(sourceDirectory, "*.*", SearchOption.AllDirectories);
        string intDir = group.Intermediate(assemblyName, targetInfo, FolderPolicy.PathType.Current);
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

    private static void GenerateCache(Installation installation, string sourceDirectory, string assemblyName, GroupDescriptor group, TargetInfo targetInfo)
    {
        string intDir = group.Intermediate(assemblyName, targetInfo, FolderPolicy.PathType.Current);

        if (Directory.Exists(intDir))
        {
            Directory.Delete(intDir, true);
        }

        Directory.CreateDirectory(intDir);

        foreach (var sourceFile in GatherSourceCodes(installation, sourceDirectory, assemblyName, group, targetInfo))
        {
            var relativeFileName = Path.GetRelativePath(sourceDirectory, sourceFile);
            var fileId = relativeFileName.Replace(Path.DirectorySeparatorChar, '_');
            var cacheFileName = Path.Combine(intDir, fileId + ".cache");
            var current = SourceCodeCache.MakeCachedSimple(sourceFile, null);
            current.SaveCached(cacheFileName);
        }
    }

    public async Task<string> CompileAsync(Installation installation, string sourceDirectory, string assemblyName, string projectDescription, GroupDescriptor group, TargetInfo targetInfo, CancellationToken cancellationToken = default)
    {
        await m_Access.WaitAsync(cancellationToken);
        try
        {
            var config = VSUtility.GetConfigName(targetInfo);
            var platform = VSUtility.GetArchitectureName(targetInfo);

            var condition = CSCondition.Parse($"$(Configuration)|$(Platform)=='{config}|{platform}'");
            var projectXml = new XmlDocument();
            projectXml.Load(projectDescription);
            var csproj = CSProject.Parse(projectXml.ChildNodes.OfType<XmlElement>().First()).Freeze(condition);

            var sourceFiles = GatherSourceCodes(installation, sourceDirectory, assemblyName, group, targetInfo)
                .Select(CSSourceCode.FromFile)
                .Append(csproj.GenerateAssemblyAttribute(null, config, null, assemblyName, Version.Parse("1.0.0.0")));

            string outputDll = await CSCompiler.CompileAsAsync(sourceFiles, csproj, Path.GetDirectoryName(projectDescription)!, assemblyName, cancellationToken);

            GenerateCache(installation, sourceDirectory, assemblyName, group, targetInfo);
            return outputDll;
        }
        catch (CSCompilerError error)
        {
            Console.Error.WriteLine("{0}\n{1}", assemblyName, error.Message);
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

    public async Task<string> CompileAsync(Installation installation, string projectFile, GroupDescriptor group, TargetInfo targetInfo, CancellationToken cancellationToken = default)
    {
        var sourceDirectory = Path.GetDirectoryName(projectFile);
        if (sourceDirectory == null)
        {
            throw new InvalidOperationException();
        }

        var assemblyName = Path.GetFileNameWithoutExtension(projectFile);
        string csproj = await File.ReadAllTextAsync(projectFile);
        return await CompileAsync(installation, sourceDirectory, assemblyName, csproj, group, targetInfo, cancellationToken);
    }
}
