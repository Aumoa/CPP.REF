// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine;

internal partial class RHTGenerator
{
    public string GenerateSourceCode(RHTGenerator.Collection collection)
    {
        List<string> headers = [];
        foreach (var aclass in Classes)
        {
            foreach (var property in aclass.Properties)
            {
                if (property.TypeName.ByRef)
                {
                    if (collection.FindMatch(property.TypeName, aclass, out var generator, out _) == false)
                    {
                        var context = property.Context;
                        throw new ParsingErrorException(context.FilePath, context.LineNumber, context.ColumnNumber, $"The requested class \"{property.TypeName.CSharp}\"'s defining header file could not be found in the Reflection header file list.");
                    }

                    headers.Add(generator.SourceCode.FilePath);
                }
            }
        }

        string headersInclude = string.Empty;
        if (headers.Count > 0)
        {
            headersInclude = "\n" + string.Join("\n", headers.Select(header => $"#include \"{header.Replace('\\', '/')}\""));
        }

        string sourceCodeText = $"""
// Copyright 2020-2025 AylaEngine. All Rights Reserved.
// This file is auto-generated. Do not edit it manually.

#include "CoreMinimal.h"{headersInclude}
#include "{SourceCode.FilePath.Replace('\\', '/')}"


""";

        foreach (var syntax in m_Syntaxes)
        {
            if (syntax is AClass aclass)
            {
                var lineNumber = aclass.LineNumber;

                string @namespace = aclass.Class.NamespaceCpp;
                string @class = aclass.Class.Name;

                sourceCodeText += $"ACLASS__IMPL_CLASS_REGISTER({@namespace}, {@class});\n";
                sourceCodeText += $"\n";

                if (aclass.Body != null)
                {
                    sourceCodeText += $"namespace {@namespace}\n";
                    sourceCodeText +=  "{\n";
                    sourceCodeText += $"  void {@class}::GatherProperties(::Ayla::PropertyCollector& collector)\n";
                    sourceCodeText +=  "  {\n";
                    sourceCodeText +=  "    Super::GatherProperties(collector);\n";
                    sourceCodeText += $"    Transfer(collector);\n";
                    sourceCodeText +=  "  }\n";
                    sourceCodeText +=  "}\n\n";
                    sourceCodeText +=  "extern \"C\"\n";
                    sourceCodeText +=  "{\n";
                    foreach (var function in aclass.Functions)
                    {
                        List<string> parameters = [];
                        List<string> arguments = [];

                        IEnumerable<string> parametersWithSelf = parameters;
                        IEnumerable<string> argumentsWithSelf = arguments;

                        if (function.Static == false)
                        {
                            parameters.Add("::Ayla::ssize_t self_");
                        }

                        foreach (var parameter in function.ParameterInfos)
                        {
                            parameters.Add($"{parameter.TypeName.CppBindingsParameter} {parameter.Name}");
                            arguments.Add(parameter.TypeName.CppBindingsArgument(aclass, parameter.Name));
                        }
                    }
                    sourceCodeText +=  "}\n";
                }
                
                sourceCodeText +=  "\n";
            }
        }

        return sourceCodeText;
    }
}
