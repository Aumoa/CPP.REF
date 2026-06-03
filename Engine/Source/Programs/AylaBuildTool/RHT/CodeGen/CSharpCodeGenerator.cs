// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine.RHT.CodeGen;

/// <summary>
/// Generator for C# reflection code.
/// </summary>
internal class CSharpCodeGenerator
{
    private readonly RHTGenerator m_Generator;
    private readonly RHTGenerationContext m_Context;
    private readonly TypeNames m_TypeNames;
    private int m_Indent;

    public CSharpCodeGenerator(RHTGenerator generator, RHTGenerationContext context, TypeNames typeNames)
    {
        m_Generator = generator;
        m_Context = context;
        m_TypeNames = typeNames;
        m_Indent = 0;
    }

    public string Generate()
    {
        string sourceCode = GenerateFileHeader();

        foreach (var syntax in m_Generator.Syntaxes)
        {
            if (syntax is SAEnum aenum)
            {
                sourceCode += GenerateEnum(aenum);
            }
            else if (syntax is SAClass aclass)
            {
                sourceCode += GenerateClass(aclass);
            }
            else if (syntax is SDefineLogCategory logCategory)
            {
                sourceCode += GenerateLogCategory(logCategory);
            }
        }

        return sourceCode;
    }

    private string GenerateFileHeader()
    {
        return """
// Copyright 2020-2025 AylaEngine. All Rights Reserved.
// This file is auto-generated. Do not edit it manually.

#nullable disable


""";
    }

    private string GenerateEnum(SAEnum aenum)
    {
        var @enum = m_TypeNames.FindEnum(aenum);
        string result = string.Empty;

        if (@enum.Namespace.Names.Length > 0)
        {
            result += IndentedLine($"namespace {string.Join(".", @enum.Namespace.Names)}");
            result += IndentedLine("{");
            Indent();
        }

        result += IndentedLine($"public enum {@enum.Name}");
        result += IndentedLine("{");
        Indented(() =>
        {
            for (int i = 0; i < aenum.Defines.Length; ++i)
            {
                var value = aenum.Defines[i];
                string comma = i < aenum.Defines.Length - 1 ? "," : string.Empty;
                if (value.Value == null)
                {
                    result += IndentedLine($"{value.Name}{comma}");
                }
                else
                {
                    result += IndentedLine($"{value.Name} = {value.Value}{comma}");
                }
            }
        });
        result += IndentedLine("}");
        result += IndentedLine("");

        if (@enum.Namespace.Names.Length > 0)
        {
            Dedent();
            result += IndentedLine("}");
        }

        return result;
    }

    private string GenerateClass(SAClass aclass)
    {
        var classGenerator = new CSharpClassGenerator(this, aclass);
        return classGenerator.Generate();
    }

    private string GenerateLogCategory(SDefineLogCategory logCategory)
    {
        var generator = new CSharpLogCategoryGenerator(this, logCategory);
        return generator.Generate();
    }

    internal string IndentedLine(string text)
    {
        return new string(' ', m_Indent * 4) + text + "\n";
    }

    internal void Indent() => ++m_Indent;
    internal void Dedent() => --m_Indent;

    internal void Indented(Action body)
    {
        ++m_Indent;
        try
        {
            body();
        }
        finally
        {
            --m_Indent;
        }
    }

    internal TypeNames TypeNames => m_TypeNames;
    internal RHTGenerationContext Context => m_Context;
    internal string ModuleName => m_Generator.SourceFile.ModuleName;
    internal ref int IndentRef => ref m_Indent;
}
