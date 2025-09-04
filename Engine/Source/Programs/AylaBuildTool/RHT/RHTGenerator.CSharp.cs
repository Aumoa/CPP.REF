using AylaEngine.RHT.Syntaxes;
using AylaEngine.RHT.Types;

namespace AylaEngine;

internal partial class RHTGenerator
{
    public string GenerateCSharp(TypeNames typeNames)
    {
        string sourceCode = $"""
// Copyright 2020-2025 AylaEngine. All Rights Reserved.
// This file is auto-generated. Do not edit it manually.

using System.Runtime.InteropServices;


""";

        int indent = 0;
        var moduleName = SourceCode.ModuleName;

        foreach (var syntax in m_Syntaxes)
        {
            if (syntax is SAClass aclass)
            {
                var @class = typeNames.FindClass(aclass.Class);
                if (@class.Namespace.Names.Length > 0)
                {
                    sourceCode += IndentedLine($"namespace {string.Join(".", @class.Namespace.Names)}");
                    sourceCode += IndentedLine($"{{");
                    ++indent;
                }

                var @base = aclass.Class.Base;
                string inherit = string.Empty;
                if (@base != null)
                {
                    var baseClass = typeNames.FindClass(@base, aclass.Class);
                    inherit = $" : {baseClass.CSharpName}";
                }

                sourceCode += IndentedLine($"public partial class {@class.Name}{inherit}");
                sourceCode += IndentedLine($"{{");

                Indented(() =>
                {
                    foreach (var field in aclass.Properties)
                    {
                        var fieldType = typeNames.FindClass(field.Variable.TypeName, aclass.Class);
                        sourceCode += IndentedLine($"[DllImport(\"{moduleName}\")]");
                        sourceCode += IndentedLine($"private static extern {fieldType.CSharpName} get_{field.Variable.Name}_Injected(nint self);");
                        sourceCode += IndentedLine($"[DllImport(\"{moduleName}\")]");
                        sourceCode += IndentedLine($"private static extern void set_{field.Variable.Name}_Injected(nint self, {fieldType.CSharpName} value);");
                    }

                    foreach (var function in aclass.Functions)
                    {
                        var returnType = typeNames.FindClass(function.ReturnType, aclass.Class);
                        var parameterTypes = function.Parameters.Select(p => typeNames.FindClass(p.Variable.TypeName, aclass.Class)).ToArray();
                        var parameterDeclare = string.Join(", ", parameterTypes.Select((t, i) => $"{t.CSharpName} {function.Parameters[i].Variable.Name}"));
                        sourceCode += IndentedLine($"[DllImport(\"{moduleName}\")]");
                        sourceCode += IndentedLine($"private static extern {returnType.CSharpName} {function.Name}_Injected(nint self{(parameterDeclare.Length > 0 ? ", " : string.Empty)}{parameterDeclare});");
                    }
                });

                sourceCode += IndentedLine($"}}");

                if (@class.Namespace.Names.Length > 0)
                {
                    --indent;
                    sourceCode += IndentedLine($"}}");
                }
            }
        }

        return sourceCode;

        void Indented(Action body)
        {
            ++indent;
            try
            {
                body();
            }
            finally
            {
                --indent;
            }
        }

        string IndentedLine(string text)
        {
            return new string(' ', indent * 4) + text + "\n";
        }
    }
}
