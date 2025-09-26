// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System.Text;
using AylaEngine.Compilation;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.Emit;
using Microsoft.CodeAnalysis.Text;

namespace AylaEngine;

public static class CSCompiler
{
    private static readonly string[] Microsoft_NET_Sdk =
    [
        "System.Private.CoreLib.dll", "System.Runtime.dll", "System.Console.dll", "System.Collections.dll",
        "System.Linq.dll", "System.Threading.dll", "System.IO.dll", "System.Net.Primitives.dll",
        "System.Private.Uri.dll", "System.Collections.Immutable.dll", "System.ObjectModel.dll",
        "System.Text.RegularExpressions.dll", "System.Private.Xml.dll", "System.Xml.ReaderWriter.dll",
        "System.Net.Http.dll"
    ];

    private static readonly string[] SdkFolders = ["Microsoft.NETCore.App", "Microsoft.AspNetCore.App", "Microsoft.WindowsDesktop.App"];

    private static readonly Version Net0900 = new(9, 0);

    private static LanguageVersion GetDefaultLangVersion(CSProject project)
    {
        switch (project.PropertyGroup.TargetFramework)
        {
            case CSTargetFramework.Net0900:
                return LanguageVersion.CSharp13;
        }

        throw new NotSupportedException($"The target framework '{project.PropertyGroup.TargetFramework}' is not supported.");
    }

    private static IEnumerable<string> GetSharedLibraries(CSProject project)
    {
        var sharedFolder = Path.GetFullPath(Path.Combine(typeof(object).Assembly.Location, "..", "..", ".."));
        string[] requiredAssemblies = project.Sdk switch
        {
            "Microsoft.NET.Sdk" => Microsoft_NET_Sdk,
            _ => throw new NotSupportedException($"The SDK '{project.Sdk}' is not supported."),
        };

        Version targetFramework = project.PropertyGroup.TargetFramework switch
        {
            CSTargetFramework.Net0900 => Net0900,
            _ => throw new NotSupportedException($"The target framework '{project.PropertyGroup.TargetFramework}' is not supported."),
        };

        List<string> candidateFolders = [];
        foreach (var sdkName in SdkFolders)
        {
            var sdkFolder = Path.Combine(sharedFolder, sdkName);
            List<Version> candidateVersions = [];
            foreach (var versionFolder in Directory.GetDirectories(sdkFolder, "*", SearchOption.TopDirectoryOnly))
            {
                var folderName = Path.GetFileName(versionFolder);
                if (Version.TryParse(folderName, out var folderVersion) == false)
                {
                    continue;
                }

                if (folderVersion.Major == targetFramework.Major && folderVersion.Minor == targetFramework.Minor)
                {
                    candidateVersions.Add(folderVersion);
                }
            }

            if (candidateVersions.Count > 0)
            {
                candidateVersions.Sort((a, b) => b.CompareTo(a));
                candidateFolders.Add(Path.Combine(sharedFolder, sdkName, candidateVersions[0].ToString(3)));
            }
        }

        foreach (var requiredAssembly in requiredAssemblies)
        {
            foreach (var candidateFolder in candidateFolders)
            {
                var assemblyPath = Path.Combine(candidateFolder, requiredAssembly);
                if (File.Exists(assemblyPath))
                {
                    yield return assemblyPath;
                    break;
                }
            }
        }
    }

    private static IEnumerable<string> GetReferencedLibraries(CSProject project, string projectDirectory, string[] referencedAssemblies)
    {
        var hashSet = referencedAssemblies.Select(r => Path.GetFileNameWithoutExtension(r)).ToHashSet();
        foreach (var reference in project.ItemGroup.References)
        {
            var referenced = reference.ReferencedAssemblyPath(project.Condition, projectDirectory, hashSet);
            if (string.IsNullOrEmpty(referenced))
            {
                continue;
            }

            yield return referenced;
        }
    }

    public static async ValueTask<CompileResult> CompileAsync(IEnumerable<CSSourceCode> sourceCodes, CSProject project, string projectDirectory, string projectName, CancellationToken cancellationToken = default)
    {
        var langVersion = GetDefaultLangVersion(project);
        var referencedAssemblies = GetSharedLibraries(project).ToArray();
        referencedAssemblies = referencedAssemblies.Concat(GetReferencedLibraries(project, projectDirectory, referencedAssemblies)).ToArray();
        if (referencedAssemblies.Any(fp => File.Exists(fp) == false))
        {
            throw new CSCompilerError("One or more required referenced assemblies are missing.");
        }

        CSharpParseOptions parseOptions = new(langVersion);
        List<Diagnostic> diagnostics = [];
        var syntaxTrees = await Task.WhenAll(sourceCodes.Select(sourceCode => Task.Run(async () =>
        {
            var instantiated = await sourceCode.InstantiateAsync(cancellationToken);
            var sourceText = SourceText.From(await instantiated.ReadContentAsync(cancellationToken), Encoding.UTF8);
            var identifier = await instantiated.GetIdentifierAsync(cancellationToken);
            var syntaxTree = CSharpSyntaxTree.ParseText(sourceText, parseOptions, identifier, cancellationToken);
            var localDiagnostics = syntaxTree.GetDiagnostics(cancellationToken).ToArray();
            lock (diagnostics)
            {
                diagnostics.AddRange(localDiagnostics);
            }

            return syntaxTree;
        })));

        var outputType = project.PropertyGroup.OutputType ?? OutputKind.DynamicallyLinkedLibrary;
        var optimized = (project.PropertyGroup.Optimize ?? true) ? OptimizationLevel.Release : OptimizationLevel.Debug;
        var metadataReferences = referencedAssemblies.Select(CreateFromFile);
        var compilerOptions = new CSharpCompilationOptions(
            outputKind: outputType,
            optimizationLevel: optimized,
            nullableContextOptions: project.PropertyGroup.Nullable ?? NullableContextOptions.Disable,
            allowUnsafe: project.PropertyGroup.AllowUnsafeBlocks ?? false
            );
        var assemblyName = project.PropertyGroup.AssemblyName ?? projectName;
        var compilation = CSharpCompilation.Create(assemblyName, syntaxTrees, metadataReferences, compilerOptions);

        using MemoryStream assemblyStream = new();
        using MemoryStream pdbStream = new();
        var emitOptions = new EmitOptions(includePrivateMembers: true, debugInformationFormat: DebugInformationFormat.PortablePdb);
        var emitResult = await Task.Run(() => compilation.Emit(assemblyStream, pdbStream, options: emitOptions, cancellationToken: cancellationToken));
        if (emitResult.Success == false)
        {
            throw new CSCompilerError(emitResult.Diagnostics);
        }

        return new CompileResult(assemblyStream.ToArray(), pdbStream.ToArray());

        PortableExecutableReference CreateFromFile(string assemblyPath)
        {
            return MetadataReference.CreateFromFile(assemblyPath);
        }
    }

    public static async ValueTask<string> CompileAsAsync(IEnumerable<CSSourceCode> sourceCodes, CSProject project, string projectDirectory, string projectName, CancellationToken cancellationToken = default)
    {
        var results = await CompileAsync(sourceCodes, project, projectDirectory, projectName, cancellationToken);
        var assemblyName = project.PropertyGroup.AssemblyName ?? projectName;
        string outputPath = project.PropertyGroup.ParseOutputPath(projectDirectory);
        Directory.CreateDirectory(outputPath);
        string assemblyFileName = Path.Combine(outputPath, assemblyName + ".dll");
        string pdbFileName = Path.Combine(outputPath, assemblyName + ".pdb");
        await Task.WhenAll(
            File.WriteAllBytesAsync(assemblyFileName, results.Assembly, cancellationToken),
            File.WriteAllBytesAsync(pdbFileName, results.Pdb, cancellationToken)
        );
        return assemblyFileName;
    }
}
