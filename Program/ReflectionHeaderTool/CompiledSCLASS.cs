// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace ReflectionHeaderTool;

internal class CompiledSCLASS : CompiledSTYPE
{
    public CompiledSCLASS(string source)
        : base(source, "SCLASS", "class")
    {
    }
}
