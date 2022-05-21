// Copyright 2020-2022 Aumoa.lib. All right reserved.

using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.Emit;
using Microsoft.CodeAnalysis.Text;

using System.Reflection;

namespace DotNETUtilities;

/// <summary>
/// 동적 컴파일에 대한 공용 함수를 제공합니다.
/// </summary>
public class DynamicCompilation
{
    /// <summary>
    /// 어셈블리를 동적으로 컴파일합니다.
    /// </summary>
    /// <param name="assemblyName"> 어셈블리 이름을 전달합니다. </param>
    /// <param name="sourceFiles"> 소스 코드 목록을 전달합니다. </param>
    /// <param name="referencedAssemblies"> 어셈블리가 참조할 어셈블리의 파일 목록을 전달합니다. </param>
    /// <param name="cancellationToken"> 작업을 취소할 수 있는 토큰을 전달합니다. </param>
    /// <returns> 컴파일 된 어셈블리가 반환됩니다. </returns>
    /// <exception cref="DynamicCompileErrorException"> 컴파일 오류가 발생했을 때 발생합니다. </exception>
    public static async Task<Assembly> CompileAssemblyAsync(string assemblyName, IEnumerable<FileReference> sourceFiles, IEnumerable<FileReference>? referencedAssemblies = null, CancellationToken cancellationToken = default)
    {
        CSharpParseOptions options = new(LanguageVersion.CSharp10);
        List<SyntaxTree> syntaxTrees = new();

        foreach (FileReference sourceFile in sourceFiles)
        {
            var source = SourceText.From(await File.ReadAllTextAsync(sourceFile.FullPath, cancellationToken));
            var syntax = CSharpSyntaxTree.ParseText(source, options, sourceFile.FullPath, cancellationToken);

            // Check compile errors.
            IEnumerable<Diagnostic> diagnostics = syntax.GetDiagnostics(cancellationToken);
            if (diagnostics.Any())
            {
                throw new DynamicCompileErrorException(assemblyName, diagnostics);
            }

            syntaxTrees.Add(syntax);
        }

        // Create metadata references.
        List<MetadataReference> metadataReferences = new();
        referencedAssemblies = Enumerable.Concat(referencedAssemblies ?? Enumerable.Empty<FileReference>(), new FileReference[]
        {
            typeof(object).AssemblyLocation(),
            new FileReference(Assembly.Load("System.Runtime").Location),
            new FileReference(Assembly.Load("System.Collections").Location)
        });

        foreach (var assembly in referencedAssemblies)
        {
            metadataReferences.Add(MetadataReference.CreateFromFile(assembly.FullPath));
        }

        // Ready compiler options.
        var compileOptions = new CSharpCompilationOptions(OutputKind.DynamicallyLinkedLibrary, true, optimizationLevel: OptimizationLevel.Release);
        var compile = CSharpCompilation.Create(assemblyName, syntaxTrees, metadataReferences, compileOptions);

        // Emit compile.
        using MemoryStream compiledAssembly = new();
        var emit = new EmitOptions(includePrivateMembers: true);
        var emitResult = compile.Emit(compiledAssembly, options: emit, cancellationToken: cancellationToken);
        if (!emitResult.Success)
        {
            throw new DynamicCompileErrorException(assemblyName, emitResult.Diagnostics);
        }

        return Assembly.Load(compiledAssembly.GetBuffer());
    }

    /// <summary>
    /// 단일 소스 파일과 단일 참조 어셈블리를 사용해 동적 어셈블리를 컴파일합니다.
    /// </summary>
    /// <typeparam name="T"> 단일 참조 어셈블리가 포함하는 아무 형식을 전달합니다. </typeparam>
    /// <param name="assemblyName"> 어셈블리 이름을 전달합니다. </param>
    /// <param name="sourceFile"> 단일 소스 파일을 전달합니다. </param>
    /// <param name="cancellationToken"> 작업을 취소할 수 있는 토큰을 전달합니다. </param>
    /// <returns> 컴파일 된 어셈블리가 반환됩니다. </returns>
    public static Task<Assembly> CompileAssemblyAsync<T>(string assemblyName, FileReference sourceFile, CancellationToken cancellationToken = default)
    {
        return CompileAssemblyAsync(
            assemblyName,
            new FileReference[] { sourceFile },
            new FileReference[] { new FileReference(typeof(T).Assembly.Location) },
            cancellationToken
            );
    }
}
