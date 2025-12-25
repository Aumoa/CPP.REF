// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine.RHT.CodeGen;

/// <summary>
/// Generator for C++ header files (.gen.h files).
/// </summary>
internal class HeaderGenerator
{
    private readonly RHTGenerator m_Generator;
    private readonly TypeNames m_TypeNames;
    private readonly string m_FileId;
    private int m_Indent;
    private string m_HeaderText = string.Empty;

    public HeaderGenerator(RHTGenerator generator, TypeNames typeNames, string fileId)
    {
        m_Generator = generator;
        m_TypeNames = typeNames;
        m_FileId = fileId;
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

        return m_HeaderText;
    }

    private void GenerateFileHeader()
    {
        m_HeaderText = $"""
// Copyright 2020-2025 AylaEngine. All Rights Reserved.
// This file is auto-generated. Do not edit it manually.

#pragma once

#include "CoreMinimal.h"

#undef GENERATED_BODY__FILE_ID__
#define GENERATED_BODY__FILE_ID__ {m_FileId}


""";
    }

    private void GenerateClass(SAClass aclass)
    {
        if (aclass.Class.Name == "Object")
        {
            return;
        }

        var classGenerator = new HeaderClassGenerator(this, aclass);
        classGenerator.Generate();
    }

    private void GenerateEnum(SAEnum aenum)
    {
        var enumGenerator = new HeaderEnumGenerator(this, aenum);
        enumGenerator.Generate();
    }

    internal string IndentedLine(string text)
    {
        return new string(' ', m_Indent * 2) + text + "\n";
    }

    internal string IndentedMacroLine(string text)
    {
        return IndentedLine(text + '\\');
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

    internal TypeNames TypeNames => m_TypeNames;
    internal string FileId => m_FileId;
    internal string HeaderText
    {
        get => m_HeaderText;
        set => m_HeaderText = value;
    }
}
