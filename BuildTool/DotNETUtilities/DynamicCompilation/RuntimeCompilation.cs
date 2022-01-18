// Copyright 2020-2021 Aumoa.lib. All right reserved.

namespace DotNETUtilities;

using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;

using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.Emit;
using Microsoft.CodeAnalysis.Text;

public static class RuntimeCompilation
{
    public static Assembly CompileAssembly(string AssemblyName, List<FileReference> SourceFiles, List<FileReference> ReferencedAssemblies, List<string> PreprocessorDefines = null)
    {
        CSharpParseOptions Options = new(LanguageVersion.Latest, preprocessorSymbols: PreprocessorDefines);
        List<SyntaxTree> SyntaxTrees = new();

        // Compile source code syntx.
        foreach (FileReference SourceFile in SourceFiles)
        {
            var Source = SourceText.From(SourceFile.ReadAllText());
            SyntaxTree Syntax = CSharpSyntaxTree.ParseText(Source, Options);

            // Check compile errors.
            IEnumerable<Diagnostic> Diagnostics = Syntax.GetDiagnostics();
            if (Diagnostics.Count() > 0)
            {
                throw new DynamicCompilationException(SourceFile.FullPath,
                Diagnostics.First().ToString());
            }

            // Add syntax to syntax tree.
            SyntaxTrees.Add(Syntax);
        }

        // Create metadata references.
        List<MetadataReference> MetadataReferences = new();
        if (ReferencedAssemblies is not null)
        {
            foreach (FileReference Reference in ReferencedAssemblies)
            {
                if (!Reference.IsExist)
                {
                    throw new FileNotFoundException($"Couldn't find referenced assembly while compile code. Assembly: '{Reference.Name}'");
                }
                MetadataReferences.Add(MetadataReference.CreateFromFile(Reference.FullPath));
            }
        }

        // Add basic referenced assemblies.
        MetadataReferences.Add(MetadataReference.CreateFromFile(typeof(object).Assembly.Location));
        MetadataReferences.Add(MetadataReference.CreateFromFile(Assembly.Load("System.Runtime").Location));
        MetadataReferences.Add(MetadataReference.CreateFromFile(Assembly.Load("System.Collections").Location));

        // Ready compiler options.
        CSharpCompilationOptions CompileOptions = new(OutputKind.DynamicallyLinkedLibrary, true, optimizationLevel: OptimizationLevel.Release);
        var Compile = CSharpCompilation.Create(AssemblyName, SyntaxTrees, MetadataReferences, CompileOptions);

        // Compile aseembly to memory stream.
        using (MemoryStream CompiledAssembly = new())
        {
            var Emit = new EmitOptions(includePrivateMembers: true);
            EmitResult Result = Compile.Emit(CompiledAssembly, options: Emit);
            if (!Result.Success)
            {
                throw new DynamicCompilationException(AssemblyName, Result.Diagnostics.First()?.ToString());
            }

            // Load assembly.
            return Assembly.Load(CompiledAssembly.GetBuffer());
        }
    }
}