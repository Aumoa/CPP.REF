﻿// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Text;

namespace ReflectionHeaderTool;

internal class CppCompilation
{
    private readonly SyntaxTree _syntaxTree;
    private readonly string _fileKey;

    public CppCompilation(SyntaxTree syntaxTree, string fileKey)
    {
        _syntaxTree = syntaxTree;
        _fileKey = fileKey;
    }

    public bool Emit(StringBuilder sb)
    {
        static bool IsSCLASS(SyntaxCore sCore)
        {
            return sCore.Type == SyntaxType.Identifier && sCore.Name == "SCLASS";
        }

        List<IHeaderGenerator> generators = new();
        for (var enumerator = _syntaxTree.GetEnumerator(); enumerator.MoveNext();)
        {
            var syntaxes = enumerator.EnumerateNext(IsSCLASS);
            if (syntaxes == null)
            {
                break;
            }

            var instance = new CompiledSCLASS();
            instance.Compile(enumerator, syntaxes);
            generators.Add(instance);
        }

        if (generators.Count == 0)
        {
            // There is no any reflection classes.
            return false;
        }

        sb.AppendLine("// Copyright 2020-2022 Aumoa.lib. All right reserved.");
        sb.AppendLine("// Generated by ReflectionHeaderTool. Do NOT edit this header file.");
        sb.AppendLine();

        foreach (var generator in generators)
        {
            generator.GenerateHeader(_fileKey, sb);
        }

        sb.AppendLine($"#undef __LIBTY_GENERATED_FILE_ID__");
        sb.AppendLine($"#define __LIBTY_GENERATED_FILE_ID__ {_fileKey}");
        return true;
    }
}
