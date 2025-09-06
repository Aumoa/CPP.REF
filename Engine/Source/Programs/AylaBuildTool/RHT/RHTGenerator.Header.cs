// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using AylaEngine.RHT.Syntaxes;
using AylaEngine.RHT.Types;

namespace AylaEngine;

internal partial class RHTGenerator
{
    public string GenerateHeader(TypeNames typeNames)
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
            if (syntax is SAClass aclass)
            {
                if (aclass.Class.Name == "Object")
                {
                    continue;
                }

                var lineNumber = aclass.Body.LineNumber;
                var scope = string.Join("::", aclass.Class.Namespaces.Select(ns => ns.Name));

                headerText += $"#define GENERATED_BODY__IMPL__{m_FileId}__{lineNumber}__reflexpr_class \\\n";
                headerText += $"struct reflexpr_class\\\n";
                headerText += $"{{\\\n";
                headerText += $"  struct aliased : public ::std::experimental::reflect::reflexpr_aliased\\\n";
                headerText += $"  {{\\\n";
                headerText += $"    static constexpr ::Ayla::String name = TEXT(\"{aclass.Class.Name}\");\\\n";
                headerText += $"    static constexpr ::Ayla::String scope = TEXT(\"{scope}\");\\\n";
                headerText += $"  }};\\\n";
                headerText += $"  \\\n";
                headerText += $"  struct members : public ::std::experimental::reflect::reflexpr_members\\\n";
                headerText += $"  {{\\\n";
                foreach (var field in aclass.Properties)
                {
                var fieldType = typeNames.FindType(field.Variable.TypeName, aclass.Class);
                headerText += $"    static consteval auto {field.Variable.Name}()\\\n";
                headerText += $"    {{\\\n";
                headerText += $"      /*{fieldType.Id} {field.Variable.Name}*/\\\n";
                headerText += $"      return ::std::experimental::reflect::reflexpr_field<decltype({aclass.Class.Name}::{field.Variable.Name}), offsetof({aclass.Class.Name}, {field.Variable.Name})>();\\\n";
                headerText += $"    }}\\\n";
                headerText += $"    \\\n";
                }
                for (int i = 0; i < aclass.Functions.Count; ++i)
                {
                var function = aclass.Functions[i];
                var returnType = typeNames.FindType(function.ReturnType, aclass.Class);
                var parameterTypes = function.Parameters
                    .Select(p => typeNames.FindType(p.Variable.TypeName, aclass.Class))
                    .ToArray();
                headerText += $"    static consteval auto {function.Name}__{i}()\\\n";
                headerText += $"    {{\\\n";
                headerText += $"      /*{returnType.Id}({aclass.Class.Name}::*{function.Name})({string.Join(", ", parameterTypes.Select(p => p.Id))})*/\\\n";
                headerText += $"      using signature_t = {function.ReturnType.FullName}({aclass.Class.Name}::*)({string.Join(", ", function.Parameters.Select(p => p.Variable.TypeName.FullName))});\\\n";
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
                headerText += $"        return {aclass.Properties[i].Variable.Name}();\\\n";
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

        return headerText;
    }
}
