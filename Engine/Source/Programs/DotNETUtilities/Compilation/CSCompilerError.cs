// Copyright 2020-2025 Aumoa.lib. All right reserved.

using Microsoft.CodeAnalysis;

namespace AylaEngine;

public class CSCompilerError : Exception
{
    public CSCompilerError(IEnumerable<Diagnostic> diagnostics)
        : base(FormatMessage(diagnostics))
    {
    }

    private static string FormatMessage(IEnumerable<Diagnostic> diagnostics)
    {
        List<string> messages = new();
        foreach (var Diag in diagnostics)
        {
            messages.Add(Diag.ToString());
        }
        return string.Join("\n", messages);
    }
}
