// Copyright 2020-2025 Aumoa.lib. All right reserved.

using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.Emit;
using Microsoft.CodeAnalysis.Text;

using System.Reflection;

namespace AylaEngine;

public static class CSCompiler
{
    public static async Task<MemoryStream> InternalCompileAsync(string assemblyName, IEnumerable<string> sourceFiles, IEnumerable<string> referencedAssemblies, CancellationToken cancellationToken = default)
    {
        CSharpParseOptions parseOptions = new(LanguageVersion.CSharp11);
        List<SyntaxTree> syntaxTrees = new();
        List<Diagnostic> compileErrors = new();

        foreach (var sourceFile in sourceFiles)
        {
            var source = SourceText.From(await File.ReadAllTextAsync(sourceFile, cancellationToken));
            var syntax = await Task.Run(() => CSharpSyntaxTree.ParseText(source, parseOptions, sourceFile, cancellationToken));

            // Check syntax error.
            IEnumerable<Diagnostic> diagnostics = syntax.GetDiagnostics(cancellationToken);
            if (diagnostics.Any())
            {
                compileErrors.AddRange(diagnostics);
            }
            else
            {
                syntaxTrees.Add(syntax);
            }
        }

        if (compileErrors.Any())
        {
            throw new CSCompilerError(compileErrors);
        }

        var metadataReferences = referencedAssemblies.Select(AssemblyPath => MetadataReference.CreateFromFile(AssemblyPath));
        var compilerOptions = new CSharpCompilationOptions(OutputKind.DynamicallyLinkedLibrary, true, optimizationLevel: OptimizationLevel.Release);
        var compilation = CSharpCompilation.Create(assemblyName, syntaxTrees, metadataReferences, compilerOptions);

        MemoryStream compiledBinary = new();
        var emitOptions = new EmitOptions(includePrivateMembers: true);
        var emitResult = await Task.Run(() => compilation.Emit(compiledBinary, options: emitOptions, cancellationToken: cancellationToken));
        if (emitResult.Success == false)
        {
            throw new CSCompilerError(emitResult.Diagnostics);
        }

        return compiledBinary;
    }

    public static async Task<Assembly> CompileAsync(string assemblyName, IEnumerable<string> sourceFiles, IEnumerable<string> referencedAssemblies, CancellationToken cancellationToken = default)
    {
        using MemoryStream compiledBinary = await InternalCompileAsync(assemblyName, sourceFiles, referencedAssemblies, cancellationToken);
        return Assembly.Load(compiledBinary.GetBuffer());
    }

    public static async Task CompileToAsync(string assemblyName, string saveTo, IEnumerable<string> sourceFiles, IEnumerable<string> referencedAssemblies, CancellationToken cancellationToken = default)
    {
        using MemoryStream compiledBinary = await InternalCompileAsync(assemblyName, sourceFiles, referencedAssemblies, cancellationToken);
        await File.WriteAllBytesAsync(saveTo, compiledBinary.GetBuffer(), cancellationToken);
    }

    public static async Task<Assembly> CompileAsync(string assemblyName, string sourceFile, IEnumerable<string> referencedAssemblies, bool includeBaseAssemblies = true, CancellationToken cancellationToken = default)
    {
        if (includeBaseAssemblies)
        {
            referencedAssemblies = referencedAssemblies.Concat(new string[]
            {
                typeof(object).Assembly.Location,
                Assembly.Load("System.Runtime").Location,
                Assembly.Load("System.Collections").Location
            });
        }

        Assembly compiledAssembly = await CompileAsync(assemblyName, new string[] { sourceFile }, referencedAssemblies, cancellationToken);
        return compiledAssembly;
    }

    public static async Task<Type> LoadClassAsync<TBaseClass>(string sourceFile, CancellationToken cancellationToken = default)
    {
        Type basedType = typeof(TBaseClass);

        string assemblyName = Path.GetFileNameWithoutExtension(sourceFile);
        Assembly compiledAssembly = await CompileAsync(assemblyName, sourceFile, new[] { basedType.Assembly.Location }, cancellationToken: cancellationToken);

        foreach (var type in compiledAssembly.GetTypes())
        {
            if (type.IsAssignableTo(basedType))
            {
                return type;
            }
        }

        throw new ClassNotFoundException(assemblyName, basedType);
    }
}
