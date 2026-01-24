// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine.RHT.CodeGen;

/// <summary>
/// Generator for C++ source code (.cpp files).
/// </summary>
internal class CppSourceGenerator
{
    private readonly RHTGenerator m_Generator;
    private readonly ModuleProject m_Project;
    private readonly TargetInfo m_BuildTarget;
    private readonly TypeNames m_TypeNames;
    private int m_Indent;
    private string m_SourceCode = string.Empty;

    public CppSourceGenerator(RHTGenerator generator, ModuleProject project, TargetInfo buildTarget, TypeNames typeNames)
    {
        m_Generator = generator;
        m_Project = project;
        m_BuildTarget = buildTarget;
        m_TypeNames = typeNames;
        m_Indent = 0;
    }

    public string Generate()
    {
        GenerateFileHeader();

        foreach (var syntax in m_Generator.Syntaxes)
        {
            if (syntax is SAClass aclass)
            {
                GenerateClass(aclass);
            }
            else if (syntax is SAEnum aenum)
            {
                GenerateEnum(aenum);
            }
        }

        return m_SourceCode;
    }

    private void GenerateFileHeader()
    {
        List<string> headers = CollectRequiredHeaders();

        string headersInclude = string.Empty;
        if (headers.Count > 0)
        {
            headersInclude = "\n" + string.Join("\n", headers.Select(header => $"#include \"{header.Replace('\\', '/')}\""));
        }

        m_SourceCode = $"""
// Copyright 2020-2025 AylaEngine. All Rights Reserved.
// This file is auto-generated. Do not edit it manually.

#include "CoreMinimal.h"
#include "Reflection/TypeCollector.h"{headersInclude}
#include "{m_Generator.SourceCode.FilePath.Replace('\\', '/')}"


""";
    }

    private List<string> CollectRequiredHeaders()
    {
        List<string> headers = [];
        
        foreach (var aclass in m_Generator.Classes)
        {
            foreach (var type in aclass
                .Properties.Select(p => p.Variable.TypeName)
                .Concat(aclass.Functions.Select(f => f.ReturnType))
                .Concat(aclass.Functions.SelectMany(f => f.Parameters.Select(p => p.Variable.TypeName)))
                .Concat(aclass.Constructors.SelectMany(f => f.Parameters.Select(p => p.Variable.TypeName))))
            {
                var typeName = m_TypeNames.FindType(type, aclass.Class);
                CollectElementType(typeName, headers);
            }
        }

        return headers;
    }

    private void CollectElementType(TypeName typeName, List<string> headers)
    {
        if (typeName is ClassName className)
        {
            headers.Add(className.Source.SourceCode.FilePath);
        }
        else if (typeName is SharedPtrTypeName rptr)
        {
            headers.Add(((ClassName)rptr.ElementType).Source.SourceCode.FilePath);
        }
        else if (typeName is ArrayTypeName array)
        {
            CollectElementType(array.ElementType, headers);
        }
    }

    private void GenerateClass(SAClass aclass)
    {
        var classGenerator = new CppClassGenerator(this, aclass);
        classGenerator.Generate();
    }

    private void GenerateEnum(SAEnum aenum)
    {
        var enumGenerator = new CppEnumGenerator(this, aenum);
        enumGenerator.Generate();
    }

    internal void WriteIndentedLine(string text)
    {
        m_SourceCode += new string(' ', m_Indent * 2) + text + "\n";
    }

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

    internal ModuleProject Project => m_Project;
    internal TypeNames TypeNames => m_TypeNames;
    internal TargetInfo BuildTarget => m_BuildTarget;
}
