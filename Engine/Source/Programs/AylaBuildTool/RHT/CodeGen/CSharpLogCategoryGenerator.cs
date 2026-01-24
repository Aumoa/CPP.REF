// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine.RHT.CodeGen;

/// <summary>
/// Generator for C# log category code.
/// </summary>
internal class CSharpLogCategoryGenerator
{
    private readonly CSharpCodeGenerator m_Parent;
    private readonly SDefineLogCategory m_LogCategory;

    public CSharpLogCategoryGenerator(CSharpCodeGenerator parent, SDefineLogCategory logCategory)
    {
        m_Parent = parent;
        m_LogCategory = logCategory;
    }

    public string Generate()
    {
        string result = string.Empty;

        result += m_Parent.IndentedLine($"namespace {string.Join(".", m_LogCategory.Namespaces.Select(n => n.Name))}");
        result += m_Parent.IndentedLine("{");
        m_Parent.Indented(() =>
        {
            result += GenerateLogCategoryClass();
        });
        result += m_Parent.IndentedLine("}");

        return result;
    }

    private string GenerateLogCategoryClass()
    {
        string result = string.Empty;

        result += m_Parent.IndentedLine($"public static class {m_LogCategory.Name}");
        result += m_Parent.IndentedLine("{");
        m_Parent.Indented(() =>
        {
            result += m_Parent.IndentedLine($"public const string kCategory = \"{m_LogCategory.Name}\";");
            result += m_Parent.IndentedLine("");

            result += GenerateLogMethod();
            result += GenerateLogWithArgsMethod();
            result += GenerateVerboseMethods();
            result += GenerateInfoMethods();
            result += GenerateWarningMethods();
            result += GenerateErrorMethods();
            result += GenerateCriticalMethods();
        });
        result += m_Parent.IndentedLine("}");

        return result;
    }

    private string GenerateLogMethod()
    {
        string result = string.Empty;
        result += m_Parent.IndentedLine($"public static void Log(global::Ayla.LogVerbosity level, string message)");
        result += m_Parent.IndentedLine("{");
        m_Parent.Indented(() =>
        {
            result += m_Parent.IndentedLine($"global::Ayla.Debug.Log(kCategory, level, message);");
        });
        result += m_Parent.IndentedLine("}");
        result += m_Parent.IndentedLine("");
        return result;
    }

    private string GenerateLogWithArgsMethod()
    {
        string result = string.Empty;
        result += m_Parent.IndentedLine($"public static void Log(global::Ayla.LogVerbosity level, string format, params global::System.ReadOnlySpan<object> args)");
        result += m_Parent.IndentedLine("{");
        m_Parent.Indented(() =>
        {
            result += m_Parent.IndentedLine($"global::Ayla.Debug.Log(kCategory, level, format, args);");
        });
        result += m_Parent.IndentedLine("}");
        result += m_Parent.IndentedLine("");
        return result;
    }

    private string GenerateVerboseMethods()
    {
        return GenerateLevelMethods("Verbose", "LogVerbose");
    }

    private string GenerateInfoMethods()
    {
        return GenerateLevelMethods("Info", "LogInfo");
    }

    private string GenerateWarningMethods()
    {
        return GenerateLevelMethods("Warning", "LogWarning");
    }

    private string GenerateErrorMethods()
    {
        return GenerateLevelMethods("Error", "LogError");
    }

    private string GenerateCriticalMethods()
    {
        return GenerateLevelMethods("Critical", "LogCritical");
    }

    private string GenerateLevelMethods(string methodName, string debugMethod)
    {
        string result = string.Empty;

        // Simple message method
        result += m_Parent.IndentedLine($"public static void {methodName}(string message)");
        result += m_Parent.IndentedLine("{");
        m_Parent.Indented(() =>
        {
            result += m_Parent.IndentedLine($"global::Ayla.Debug.{debugMethod}(kCategory, message);");
        });
        result += m_Parent.IndentedLine("}");
        result += m_Parent.IndentedLine("");

        // Format with args method
        result += m_Parent.IndentedLine($"public static void {methodName}(string format, params System.ReadOnlySpan<object> args)");
        result += m_Parent.IndentedLine("{");
        m_Parent.Indented(() =>
        {
            result += m_Parent.IndentedLine($"global::Ayla.Debug.{debugMethod}(kCategory, format, args);");
        });
        result += m_Parent.IndentedLine("}");
        result += m_Parent.IndentedLine("");

        return result;
    }
}
