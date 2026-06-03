// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using AylaEngine.RHT.CodeGen;

namespace AylaEngine.RHT;

internal partial class RHTGenerator
{
    public string GenerateSourceCode(RHTGenerationContext context, TypeNames typeNames)
    {
        var generator = new CppSourceGenerator(this, context, typeNames);
        return generator.Generate();
    }
}
