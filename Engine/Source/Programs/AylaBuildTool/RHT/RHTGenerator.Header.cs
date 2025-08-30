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
                    headerText += $"  struct aliased : public ::std::experimental::reflect::reflexpr_aliased\\\n";
                    headerText += $"  {{\\\n";
                    headerText += $"    static constexpr ::Ayla::String name = TEXT(\"{aclass.Class.Name}\");\\\n";
                    headerText += $"    static constexpr ::Ayla::String scope = TEXT(\"{aclass.Class.NamespaceCpp}\");\\\n";
                    headerText += $"  }};\\\n";
                    headerText += $"  \\\n";
                    headerText += $"  struct members : public ::std::experimental::reflect::reflexpr_members\\\n";
                    headerText += $"  {{\\\n";
                    foreach (var field in aclass.Properties)
                    {
                    headerText += $"    static consteval auto {field.Name}()\\\n";
                    headerText += $"    {{\\\n";
                    headerText += $"      return ::std::experimental::reflect::reflexpr_field<{field.TypeName.CppBindings}, offsetof({aclass.Class.Name}, {field.Name})>();\\\n";
                    headerText += $"    }}\\\n";
                    headerText += $"    \\\n";
                    }
                    for (int i = 0; i < aclass.Functions.Count; ++i)
                    {
                    var function = aclass.Functions[i];
                    headerText += $"    static consteval auto {function.Name}__{i}()\\\n";
                    headerText += $"    {{\\\n";
                    headerText += $"      using signature_t = {function.Return.CppBindings}({aclass.Class.Name}::*)({string.Join(", ", function.ParameterInfos.Select(p => p.TypeName.CppBindings))});\\\n";
                    headerText += $"      return ::std::experimental::reflect::reflexpr_method<signature_t, (signature_t)&{aclass.Class.Name}::{function.Name}>();\\\n";
                    headerText += $"    }}\\\n";
                    headerText += $"    \\\n";
                    }
                    headerText += $"    template<size_t N>\\\n";
                    headerText += $"    static consteval auto get() noexcept\\\n";
                    headerText += $"    {{\\\n";
                    for (int i = 0; i < aclass.Properties.Count; ++i)
                    {
                    headerText += $"      if constexpr (N == {i})\\\n";
                    headerText += $"      {{\\\n";
                    headerText += $"        return {aclass.Properties[i].Name}();\\\n";
                    headerText += $"      }}\\\n";
                    }
                    for (int i = 0; i < aclass.Functions.Count; ++i)
                    {
                    headerText += $"      if constexpr (N == {(aclass.Properties.Count + i)})\\\n";
                    headerText += $"      {{\\\n";
                    headerText += $"        return {aclass.Functions[i].Name}__{i}();\\\n";
                    headerText += $"      }}\\\n";
                    }
                    if (aclass.Properties.Count + aclass.Functions.Count > 0)
                    {
                    headerText += $"      else\\\n";
                    }
                    headerText += $"      {{\\\n";
                    headerText += $"        static_assert(N < {aclass.Properties.Count + aclass.Functions.Count}, \"Invalid reflexpr index\");\\\n";
                    headerText += $"      }}\\\n";
                    headerText += $"    }}\\\n";
                    headerText += $"    \\\n";
                    headerText += $"    static constexpr size_t tuple_size = {aclass.Properties.Count + aclass.Functions.Count};\\\n";
                    headerText += $"  }};\\\n";
                    headerText += $"}};\n";
                    headerText += $"\n";
                    headerText += $"#define GENERATED_BODY__IMPL__{m_FileId}__{lineNumber} \\\n";
                    headerText += $"  GENERATED_BODY__DEFAULT_BODY({aclass.Class.Name}) \\\n";
                    headerText += $"  GENERATED_BODY__IMPL__{m_FileId}__{lineNumber}__reflexpr_class\\\n";
                    headerText += $"  GENERATED_BODY__DECLARE_GATHER_PROPERTIES()\\\n";
                    headerText += $"  GENERATED_BODY__DECLARE_TRANSFER()\\\n";
                    headerText += $"private:\n";
                }
            }
        }

        return headerText;
    }
}
