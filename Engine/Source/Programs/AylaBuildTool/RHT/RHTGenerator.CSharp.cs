// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine;

internal partial class RHTGenerator
{
    public string GenerateBindings()
    {
        string sourceCodeText = $"""
// Copyright 2020-2025 AylaEngine. All Rights Reserved.
// This file is auto-generated. Do not edit it manually.

using System.Runtime.InteropServices;


""";

        foreach (var syntax in m_Syntaxes)
        {
            if (syntax is AClass aclass)
            {
                string @namespace = aclass.Class.Namespace;
                string @class = aclass.Class.Name;
                string inherit;
                if (string.IsNullOrEmpty(aclass.Class.BaseClass) == false)
                {
                    inherit = $" : {aclass.Class.BaseClass}";
                }
                else
                {
                    inherit = " : global::System.IDisposable";
                }

                sourceCodeText += $"namespace {@namespace}\n";
                sourceCodeText += "{\n";
                sourceCodeText += $"\tpublic class {@class}{inherit}\n";
                sourceCodeText += "\t{\n";
                if (string.IsNullOrEmpty(aclass.Class.BaseClass))
                {
                    sourceCodeText += "\t\tprivate nint m_InstancePtr;\n";
                    sourceCodeText += "\t\t\n";
                    sourceCodeText += "\t\tpublic long InstancePtr => m_InstancePtr;\n";
                    sourceCodeText += "\t\t\n";
                    sourceCodeText += $"\t\tpublic {@class}()\n";
                    sourceCodeText += "\t\t{\n";
                    sourceCodeText += $"\t\t\tm_InstancePtr = {FunctionName1("New")}();\n";
                    sourceCodeText += "\t\t}\n";
                    sourceCodeText += "\t\t\n";
                    sourceCodeText += $"\t\tprotected {@class}(nint instancePtr)\n";
                    sourceCodeText += "\t\t{\n";
                    sourceCodeText += "\t\t\tm_InstancePtr = instancePtr;\n";
                    sourceCodeText += "\t\t}\n";
                    sourceCodeText += "\t\t\n";
                    sourceCodeText += $"\t\t~{@class}()\n";
                    sourceCodeText += "\t\t{\n";
                    sourceCodeText += "\t\t\tDispose(false);\n";
                    sourceCodeText += "\t\t}\n";
                    sourceCodeText += "\t\t\n";
                    sourceCodeText += "\t\tpublic void Dispose()\n";
                    sourceCodeText += "\t\t{\n";
                    sourceCodeText += "\t\t\tDispose(true);\n";
                    sourceCodeText += "\t\t\tglobal::System.GC.SuppressFinalize(this);\n";
                    sourceCodeText += "\t\t}\n";
                    sourceCodeText += "\t\t\n";
                    sourceCodeText += "\t\tprotected virtual void Dispose(bool disposing)\n";
                    sourceCodeText += "\t\t{\n";
                    sourceCodeText += $"\t\t\t{FunctionName1("Finalize")}(m_InstancePtr);\n";
                    sourceCodeText += "\t\t\tm_InstancePtr = 0;\n";
                    sourceCodeText += "\t\t}\n";
                    sourceCodeText += "\t\t\n";
                }
                else
                {
                    sourceCodeText += $"\t\tpublic {@class}() : base({FunctionName1("New")}())\n";
                    sourceCodeText += "\t\t{\n";
                    sourceCodeText += "\t\t}\n";
                    sourceCodeText += "\t\t\n";
                    sourceCodeText += $"\t\tprotected {@class}(nint instancePtr) : base(instancePtr)\n";
                    sourceCodeText += "\t\t{\n";
                    sourceCodeText += "\t\t}\n";
                    sourceCodeText += "\t\t\n";
                }
                sourceCodeText += $"\t\t[DllImport(\"{m_SourceCode.ModuleName}\")]\n";
                sourceCodeText += $"\t\tprivate static extern nint {FunctionName1("New")}();\n";
                sourceCodeText += "\t\t\n";
                sourceCodeText += $"\t\t[DllImport(\"{m_SourceCode.ModuleName}\")]\n";
                sourceCodeText += $"\t\tprivate static extern void {FunctionName1("Finalize")}(nint self_);\n";
                foreach (var function in aclass.Functions)
                {
                    List<string> parameters = [];
                    List<string> arguments = [];

                    IEnumerable<string> parametersWithSelf = parameters;
                    IEnumerable<string> argumentsWithSelf = arguments;
                    string prefix = string.Empty;

                    foreach (var parameter in function.ParameterInfos)
                    {
                        parameters.Add($"{FormatTypeToCSharp(function.Context, parameter.TypeName)} {parameter.Name}{(parameter.DefaultValue == null ? string.Empty : $" = {parameter.DefaultValue}")}");
                        arguments.Add(parameter.Name);
                    }

                    if (function.Static == false)
                    {
                        parametersWithSelf = Enumerable.Repeat("nint self_", 1).Concat(parameters);
                        argumentsWithSelf = Enumerable.Repeat("(nint)InstancePtr", 1).Concat(arguments);
                    }
                    else
                    {
                        prefix += "static ";
                    }

                    if (function.Virtual)
                    {
                        prefix += "virtual ";
                    }

                    sourceCodeText += "\t\t\n";
                    sourceCodeText += $"\t\tpublic {prefix}{FormatReturnType1()} {function.Name}({string.Join(", ", parameters)})\n";
                    sourceCodeText += "\t\t{\n";
                    sourceCodeText += $"\t\t\t{ReturnSyntax()}{FunctionName1(function.Name)}({string.Join(", ", argumentsWithSelf)});\n";
                    sourceCodeText += "\t\t}\n";
                    sourceCodeText += "\t\t\n";
                    sourceCodeText += "\t\t\n";
                    sourceCodeText += $"\t\t[DllImport(\"{m_SourceCode.ModuleName}\")]\n";
                    sourceCodeText += $"\t\tprivate static extern {FormatReturnType1()} {FunctionName1(function.Name)}({string.Join(", ", parametersWithSelf)});\n";

                    string FormatReturnType1() => FormatTypeToCSharp(function.Context, function.Return);
                    string ReturnSyntax() => function.Return == "void" ? string.Empty : "return ";
                }
                sourceCodeText += "\t}\n";
                sourceCodeText += "}\n";
                sourceCodeText += "\n";

                string FunctionName1(string name) => FunctionName(@namespace, @class, name);
            }
        }

        return sourceCodeText;
    }
}
