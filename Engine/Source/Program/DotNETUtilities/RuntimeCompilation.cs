// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;

using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.Emit;
using Microsoft.CodeAnalysis.Text;

/// <summary>
/// 실행 시간에 특정 코드를 동적으로 컴파일합니다.
/// </summary>
public static class RuntimeCompilation
{
    /// <summary>
    /// 파일 레퍼런스에 대한 모든 참조를 컴파일하여 어셈블리를 생성합니다.
    /// </summary>
    /// <param name="AssemblyName"> 컴파일 될 어셈블리의 이름을 전달합니다. </param>
    /// <param name="SourceFiles"> 컴파일 할 소스 파일 레퍼런스 목록을 전달합니다. </param>
    /// <param name="ReferencedAssemblies"> 참조해야 할 레퍼런스 어셈블리 목록을 전달합니다. </param>
    /// <param name="PreprocessorDefines"> 전처리기 정의를 전달합니다. </param>
    /// <returns> 컴파일 된 레퍼런스가 반환됩니다. </returns>
    public static Assembly CompileAssembly(string AssemblyName, List<FileReference> SourceFiles, List<FileReference> ReferencedAssemblies, List<string> PreprocessorDefines = null)
    {
        CSharpParseOptions Options = new(LanguageVersion.Latest, preprocessorSymbols: PreprocessorDefines);
        List<SyntaxTree> SyntaxTrees = new();

        // 소스 코드의 구문을 컴파일합니다.
        foreach (FileReference SourceFile in SourceFiles)
        {
            var Source = SourceText.From(SourceFile.ReaeAllText());
            SyntaxTree Syntax = CSharpSyntaxTree.ParseText(Source, Options);

            // 소스 코드 컴파일 중 구문 오류가 없는지 확인합니다.
            IEnumerable<Diagnostic> Diagnostics = Syntax.GetDiagnostics();
            if (Diagnostics.Count() > 0)
            {
                throw new DynamicCompilationException(SourceFile.FullPath,
                Diagnostics.First().ToString());
            }

            // 빌드된 구문 트리에 추가합니다.
            SyntaxTrees.Add(Syntax);
        }

        // 지정한 참조 레퍼런스들을 목록에 추가합니다.
        List<MetadataReference> MetadataReferences = new();
        if (ReferencedAssemblies is not null)
        {
            foreach (FileReference Reference in ReferencedAssemblies)
            {
                if (!Reference.IsExist)
                {
                    throw new FileNotFoundException($"어셈블리를 컴파일 하는 중 참조된 어셈블리({Reference.Name})를 찾을 수 없습니다.");
                }
                MetadataReferences.Add(MetadataReference.CreateFromFile(Reference.FullPath));
            }
        }

        // 기본적으로 포함되어야 할 어셈블리를 목록에 추가합니다.
        MetadataReferences.Add(MetadataReference.CreateFromFile(typeof(object).Assembly.Location));
        MetadataReferences.Add(MetadataReference.CreateFromFile(Assembly.Load("System.Runtime").Location));
        MetadataReferences.Add(MetadataReference.CreateFromFile(Assembly.Load("System.Collections").Location));

        // 컴파일러 옵션을 준비합니다.
        CSharpCompilationOptions CompileOptions = new(OutputKind.DynamicallyLinkedLibrary, true, optimizationLevel: OptimizationLevel.Release);
        var Compile = CSharpCompilation.Create(AssemblyName, SyntaxTrees, MetadataReferences, CompileOptions);

        // 메모리 스트림에 컴파일 데이터를 씁니다.
        using (MemoryStream CompiledAssembly = new())
        {
            var Emit = new EmitOptions(includePrivateMembers: true);
            EmitResult Result = Compile.Emit(CompiledAssembly, options: Emit);
            if (!Result.Success)
            {
                throw new DynamicCompilationException(AssemblyName, Result.Diagnostics.First()?.ToString());
            }

            // 컴파일 된 어셈블리 데이터를 이용해 어셈블리를 로드합니다.
            return Assembly.Load(CompiledAssembly.GetBuffer());
        }
    }
}