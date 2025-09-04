// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using AylaEngine.RHT.Syntaxes;
using AylaEngine.RHT.Types;

namespace AylaEngine;

internal partial class RHTGenerator
{
    public string GenerateSourceCode(TypeNames collection)
    {
        List<string> headers = [];
        foreach (var aclass in Classes)
        {
            foreach (var property in aclass.Properties)
            {
                var type = collection.FindClass(property.Variable.TypeName, aclass.Class);
                if (type is ClassName className)
                {
                    headers.Add(className.Source.SourceCode.FilePath);
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
            if (syntax is SAClass aclass)
            {
                var lineNumber = aclass.LineNumber;

                string @namespace = string.Join("::", aclass.Class.Namespaces.Select(p => p.Name));
                string @class = aclass.Class.Name;

                sourceCodeText += $"ACLASS__IMPL_CLASS_REGISTER({@namespace}, {@class});\n";
                sourceCodeText += $"\n";

                if (@class == "Object")
                {
                    continue;
                }

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
                sourceCodeText +=  "}\n";
                
                sourceCodeText +=  "\n";
            }
        }

        return sourceCodeText;
    }
}
