// Copyright 2020-2022 Aumoa.lib. All right reserved.

using Microsoft.CodeAnalysis;

namespace AE.Compilation;

public class CSCompilerError : Exception
{
    public CSCompilerError(IEnumerable<Diagnostic> Diagnostics)
        : base(FormatMessage(Diagnostics))
    {
    }

    private static string FormatMessage(IEnumerable<Diagnostic> Diagnostics)
    {
        List<string> Messages = new();
        foreach (var Diag in Diagnostics)
        {
            Messages.Add(Diag.ToString());
        }
        return string.Join("\n", Messages);
    }
}
