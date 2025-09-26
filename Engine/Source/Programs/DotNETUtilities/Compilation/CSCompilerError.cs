// Copyright 2020-2025 Aumoa.lib. All right reserved.

using Microsoft.CodeAnalysis;

namespace AylaEngine;

public class CSCompilerError : Exception
{
    public CSCompilerError(IEnumerable<Diagnostic> diagnostics)
        : base(FormatMessage(diagnostics))
    {
    }

    public CSCompilerError(string message) : base(message)
    {
    }

    private static string FormatMessage(IEnumerable<Diagnostic> diagnostics)
    {
        List<string> messages = new();
        foreach (var diag in diagnostics.Where(p => p.Severity is DiagnosticSeverity.Warning or DiagnosticSeverity.Error))
        {
            messages.Add(diag.ToString());
        }
        return string.Join("\n", messages);
    }
}
