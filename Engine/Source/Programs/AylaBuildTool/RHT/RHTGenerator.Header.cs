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

                    headerText += $"#define GENERATED_BODY__IMPL__{m_FileId}__{lineNumber} \\\n";
                    headerText += $"\tGENERATED_BODY__DEFAULT_BODY({aclass.Class.Name}) \\\n";
                    headerText += $"\tGENERATED_BODY__DECLARE_GATHER_PROPERTIES()\n\n";
                }
            }
        }

        return headerText;
    }
}
