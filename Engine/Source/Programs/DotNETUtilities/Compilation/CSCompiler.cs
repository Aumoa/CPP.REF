// Copyright 2020-2022 Aumoa.lib. All right reserved.

using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.Emit;
using Microsoft.CodeAnalysis.Text;

using System.Reflection;

namespace AE.Compilation;

public static class CSCompiler
{
    public static async Task<Assembly> CompileAsync(string AssemblyName, IEnumerable<string> SourceFiles, IEnumerable<string> ReferencedAssemblies, CancellationToken CToken = default)
    {
        CSharpParseOptions ParseOptions = new(LanguageVersion.CSharp10);
        List<SyntaxTree> SyntaxTrees = new();
        List<Diagnostic> CompileErrors = new();

        foreach (var SourceFile in SourceFiles)
        {
            var Source = SourceText.From(await File.ReadAllTextAsync(SourceFile, CToken));
            var Syntax = await Task.Run(() => CSharpSyntaxTree.ParseText(Source, ParseOptions, SourceFile, CToken));

            // Check syntax error.
            IEnumerable<Diagnostic> Diagnostics = Syntax.GetDiagnostics(CToken);
            if (Diagnostics.Any())
            {
                CompileErrors.AddRange(Diagnostics);
            }
            else
            {
                SyntaxTrees.Add(Syntax);
            }
        }

        if (CompileErrors.Any())
        {
            throw new CSCompilerError(CompileErrors);
        }

        var MetadataReferences = ReferencedAssemblies.Select(AssemblyPath => MetadataReference.CreateFromFile(AssemblyPath));
        var CompilerOptions = new CSharpCompilationOptions(OutputKind.DynamicallyLinkedLibrary, true, optimizationLevel: OptimizationLevel.Release);
        var Compilation = CSharpCompilation.Create(AssemblyName, SyntaxTrees, MetadataReferences, CompilerOptions);

        using MemoryStream CompiledBinary = new();
        var EmitOptions = new EmitOptions(includePrivateMembers: true);
        var EmitResult = await Task.Run(() => Compilation.Emit(CompiledBinary, options: EmitOptions, cancellationToken: CToken));
        if (EmitResult.Success == false)
        {
            throw new CSCompilerError(EmitResult.Diagnostics);
        }

        return Assembly.Load(CompiledBinary.GetBuffer());
    }

    public static async Task<Assembly> CompileAsync(string AssemblyName, string SourceFile, IEnumerable<string> ReferencedAssemblies, bool bIncludeBaseAssemblies = true, CancellationToken CToken = default)
    {
        if (bIncludeBaseAssemblies)
        {
            ReferencedAssemblies = ReferencedAssemblies.Concat(new string[]
            {
                typeof(object).Assembly.Location,
                Assembly.Load("System.Runtime").Location,
                Assembly.Load("System.Collections").Location
            });
        }

        Assembly CompiledAssembly = await CompileAsync(AssemblyName, new string[] { SourceFile }, ReferencedAssemblies, CToken);
        return CompiledAssembly;
    }

    public static async Task<Type> LoadClassAsync<TBaseClass>(string SourceFile, CancellationToken CToken = default)
    {
        Type BasedType = typeof(TBaseClass);

        string AssemblyName = Path.GetFileNameWithoutExtension(SourceFile);
        Assembly CompiledAssembly = await CompileAsync(AssemblyName, SourceFile, new[] { BasedType.Assembly.Location }, CToken: CToken);

        foreach (var Type in CompiledAssembly.GetTypes())
        {
            if (Type.IsAssignableTo(BasedType))
            {
                return Type;
            }
        }

        throw new ClassNotFoundException(AssemblyName, BasedType);
    }
}
