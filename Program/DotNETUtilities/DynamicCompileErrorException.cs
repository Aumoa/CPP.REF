// Copyright 2020-2022 Aumoa.lib. All right reserved.

using Microsoft.CodeAnalysis;

namespace DotNETUtilities;

/// <summary>
/// <see cref="DynamicCompilation"/> 기능의 컴파일 과정에서 발생한 오류를 수집합니다.
/// </summary>
public class DynamicCompileErrorException : Exception
{
    /// <summary>
    /// 오류 기록을 참고하여 예외 개체를 생성합니다.
    /// </summary>
    /// <param name="assemblyName"> 예외 발생 어셈블리 이름을 전달합니다. </param>
    /// <param name="diagnostics"> 오류 기록을 전달합니다. </param>
    public DynamicCompileErrorException(string assemblyName, IEnumerable<Diagnostic> diagnostics)
        : base(FormatMessage(assemblyName, diagnostics))
    {
    }

    private static string FormatMessage(string assemblyName, IEnumerable<Diagnostic> diagnostics)
    {
        return string.Format("동적 어셈블리 '{0}'을(를) 컴파일하는 중 오류가 발생했습니다.\n{1}",
            assemblyName,
            string.Join("\n", diagnostics.Select(p => p.ToString()))
            );
    }
}
