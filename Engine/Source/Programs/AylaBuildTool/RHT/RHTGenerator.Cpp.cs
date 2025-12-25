// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using AylaEngine.RHT.CodeGen;

namespace AylaEngine;

internal partial class RHTGenerator
{
    public string GenerateSourceCode(ModuleProject project, TargetInfo buildTarget, TypeNames typeNames)
    {
        var generator = new CppSourceGenerator(this, project, buildTarget, typeNames);
        return generator.Generate();
    }
}
