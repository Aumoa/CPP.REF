﻿// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Text;

namespace ReflectionHeaderTool;

internal class CppCompilation
{
    private readonly string _source;
    private readonly SyntaxTree _syntaxTree;
    private readonly string _fileKey;

    public CppCompilation(string source, SyntaxTree syntaxTree, string fileKey)
    {
        _source = source;
        _syntaxTree = syntaxTree;
        _fileKey = fileKey;
    }

    public bool Emit(StringBuilder sbHeader, StringBuilder sbSource)
    {
        static bool IsSCLASS(SyntaxCore sCore)
        {
            return sCore.Type == SyntaxType.Identifier && sCore.Name == "SCLASS";
        }

        static bool IsSINTERFACE(SyntaxCore sCore)
        {
            return sCore.Type == SyntaxType.Identifier && sCore.Name == "SINTERFACE";
        }

        List<IHeaderGenerator> generators = new();
        List<SyntaxCore> syntaxes = new();
        for (var enumerator = _syntaxTree.GetEnumerator(); enumerator.MoveNext();)
        {
            if (IsSCLASS(enumerator.Current))
            {
                syntaxes.Add(enumerator.Current);
                enumerator.MoveNext();
                var instance = new CompiledSCLASS(_source);
                instance.Compile(enumerator, syntaxes);
                generators.Add(instance);
                syntaxes.Clear();
            }
            else if (IsSINTERFACE(enumerator.Current))
            {
                syntaxes.Add(enumerator.Current);
                enumerator.MoveNext();
                var instance = new CompiledSINTERFACE(_source);
                instance.Compile(enumerator, syntaxes);
                generators.Add(instance);
                syntaxes.Clear();
            }
            else
            {
                syntaxes.Add(enumerator.Current);
            }
        }

        if (generators.Count == 0)
        {
            // There is no any reflection classes.
            return false;
        }

        sbHeader.AppendLine("// Copyright 2020-2022 Aumoa.lib. All right reserved.");
        sbHeader.AppendLine("// Generated by ReflectionHeaderTool. Do NOT edit this header file.");
        sbHeader.AppendLine();

        sbSource.AppendLine("// Copyright 2020-2022 Aumoa.lib. All right reserved.");
        sbSource.AppendLine("// Generated by ReflectionHeaderTool. Do NOT edit this source file.");
        sbSource.AppendLine();

        foreach (var generator in generators)
        {
            generator.GenerateHeader(_fileKey, sbHeader);
            generator.GenerateSource(_fileKey, sbSource);
        }

        sbHeader.AppendLine($"#undef __LIBTY_GENERATED_FILE_ID__");
        sbHeader.AppendLine($"#define __LIBTY_GENERATED_FILE_ID__ {_fileKey}");
        return true;
    }
}
