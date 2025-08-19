// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine;

internal partial class RHTGenerator
{
    public string GenerateHeader()
    {
        string headerText = $"""
// Copyright 2020-2025 AylaEngine. All Rights Reserved.
// This file is auto-generated. Do not edit it manually.

#pragma once

#include "Reflection/ReflectionMacros.h"

#undef GENERATED_BODY__FILE_ID__
#define GENERATED_BODY__FILE_ID__ {m_FileId}


""";

        foreach (var syntax in m_Syntaxes)
        {
            if (syntax is AClass aclass)
            {
                var lineNumber = aclass.LineNumber;

                if (aclass.Body != null)
                {
                    lineNumber = aclass.Body.LineNumber;

                    headerText += $"#define GENERATED_BODY__IMPL__{m_FileId}__{lineNumber}__reflexpr_class \\\n";
                    headerText += $"struct reflexpr_class\\\n";
                    headerText += $"{{\\\n";
                    headerText += $"  struct aliased\\\n";
                    headerText += $"  {{\\\n";
                    headerText += $"    static constexpr ::Ayla::String name = TEXT(\"{aclass.Class.Name}\");\\\n";
                    headerText += $"    static constexpr ::Ayla::String scope = TEXT(\"{aclass.Class.NamespaceCpp}\");\\\n";
                    headerText += $"  }};\\\n";
                    headerText += $"  \\\n";
                    headerText += $"  struct members\\\n";
                    headerText += $"  {{\\\n";
                    foreach (var field in aclass.Properties)
                    {
                        headerText += $"    static constexpr ::std::experimental::reflect::reflexpr_field<{field.TypeName.Cpp}> {field.Name} = {{}};\\\n";
                    }
                    headerText += $"    \\\n";
                    headerText += $"    template<size_t N>\\\n";
                    headerText += $"    static consteval auto get() noexcept\\\n";
                    headerText += $"    {{\\\n";
                    for (int i = 0; i < aclass.Properties.Count; ++i)
                    {
                        headerText += $"      if constexpr (N == {i})\\\n";
                        headerText += $"      {{\\\n";
                        headerText += $"        return {aclass.Properties[i].Name};\\\n";
                        headerText += $"      }}\\\n";
                    }
                    if (aclass.Properties.Count > 0)
                    {
                        headerText += $"      else\\\n";
                    }
                    headerText += $"      {{\\\n";
                    headerText += $"        static_assert(N != N, \"Invalid reflexpr index\");\\\n";
                    headerText += $"      }}\\\n";
                    headerText += $"    }}\\\n";
                    headerText += $"    \\\n";
                    headerText += $"    static constexpr size_t tuple_size = {aclass.Properties.Count};\\\n";
                    headerText += $"  }};\\\n";
                    headerText += $"}};\n";
                    headerText += $"\n";
                    headerText += $"#define GENERATED_BODY__IMPL__{m_FileId}__{lineNumber} \\\n";
                    headerText += $"  GENERATED_BODY__DEFAULT_BODY({aclass.Class.Name}) \\\n";
                    headerText += $"  GENERATED_BODY__IMPL__{m_FileId}__{lineNumber}__reflexpr_class\\\n";
                    headerText += $"  GENERATED_BODY__DECLARE_GATHER_PROPERTIES()\n\n";
                }
            }
        }

        return headerText;
    }
}
