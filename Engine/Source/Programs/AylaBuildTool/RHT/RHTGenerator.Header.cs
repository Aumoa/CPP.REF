// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using AylaEngine.RHT.CodeGen;

namespace AylaEngine;

internal partial class RHTGenerator
{
    public string GenerateHeader(TypeNames typeNames)
    {
        var generator = new HeaderGenerator(this, typeNames, m_FileId);
        return generator.Generate();
    }
}
