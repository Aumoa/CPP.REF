// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine;

internal partial class RHTGenerator
{
    public string GenerateBindings(Collection collection)
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
                string @namespace = aclass.Class.NamespaceCpp;
                string @class = aclass.Class.Name;

                if (collection.FindMatch(TypeName.FromInherit(aclass.Context, aclass.Class.BaseClass), aclass, out _, out var baseClass) == false)
                {
                    var context = aclass.Context;
                    throw new ParsingErrorException(context.FilePath, context.LineNumber, context.ColumnNumber, $"The requested class \"{aclass.Class.BaseClass}\"'s defining header file could not be found in the Reflection header file list.");
                }

                string inherit = $" : {baseClass.Class.FullNameCSharp}";

                sourceCodeText += $"namespace {@namespace}\n";
                sourceCodeText += "{\n";
                sourceCodeText += $"\tpublic class {@class}{inherit}\n";
                sourceCodeText += "\t{\n";
                sourceCodeText += $"\t\tpublic {@class}() : base({FunctionName1("New")}())\n";
                sourceCodeText += "\t\t{\n";
                sourceCodeText += "\t\t}\n";
                sourceCodeText += "\t\t\n";
                sourceCodeText += $"\t\tprotected {@class}(nint instancePtr) : base(instancePtr)\n";
                sourceCodeText += "\t\t{\n";
                sourceCodeText += "\t\t}\n";
                sourceCodeText += "\t\t\n";
                sourceCodeText += $"\t\t[DllImport(\"{SourceCode.ModuleName}\", CharSet = CharSet.Unicode)]\n";
                sourceCodeText += $"\t\tprivate static extern nint {FunctionName1("New")}();\n";
                sourceCodeText += "\t\t\n";
                sourceCodeText += $"\t\t[DllImport(\"{SourceCode.ModuleName}\", CharSet = CharSet.Unicode)]\n";
                sourceCodeText += $"\t\tprivate static extern void {FunctionName1("Finalize")}(nint self_);\n";
                foreach (var function in aclass.Functions)
                {
                    List<string> parameters1 = [];
                    List<string> parameters2 = [];
                    List<string> arguments = [];

                    IEnumerable<string> parametersWithSelf = parameters2;
                    IEnumerable<string> argumentsWithSelf = arguments;
                    string prefix = string.Empty;

                    foreach (var parameter in function.ParameterInfos)
                    {
                        parameters1.Add($"{parameter.TypeName.CSharpBindingParameter(aclass)} {parameter.Name}{(parameter.DefaultValue == null ? string.Empty : $" = {parameter.DefaultValue}")}");
                        parameters2.Add($"{parameter.TypeName.Injected} {parameter.Name}");
                        arguments.Add(parameter.TypeName.CSharpBindingArgument(parameter.Name));
                    }

                    if (function.Static == false)
                    {
                        parametersWithSelf = Enumerable.Repeat("nint self_", 1).Concat(parameters2);
                        argumentsWithSelf = Enumerable.Repeat("global::Ayla.Marshal.GetInstancePointer(this)", 1).Concat(arguments);
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
                    sourceCodeText += $"\t\tpublic {prefix}{function.Return.CSharp} {function.Name}({string.Join(", ", parameters1)})\n";
                    sourceCodeText += "\t\t{\n";
                    sourceCodeText += $"\t\t\t{ReturnSyntax()}{FunctionName1(function.Name)}({string.Join(", ", argumentsWithSelf)});\n";
                    sourceCodeText += "\t\t}\n";
                    sourceCodeText += "\t\t\n";
                    sourceCodeText += "\t\t\n";
                    sourceCodeText += $"\t\t[DllImport(\"{SourceCode.ModuleName}\", CharSet = CharSet.Unicode)]\n";
                    sourceCodeText += $"\t\tprivate static extern {function.Return.CSharp} {FunctionName1(function.Name)}({string.Join(", ", parametersWithSelf)});\n";

                    string ReturnSyntax() => function.Return.IsVoid ? string.Empty : "return ";
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
