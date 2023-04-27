// Copyright 2020-2023 Aumoa.lib. All right reserved.

namespace AE.Compile;

public record CppModuleDescriptor
{
    public required string InterfaceName { get; set; }

    public required string? SourcePath { get; set; }

    public required bool IsInterface { get; set; }

    public required ModuleLookupMethod? LookupMethod { get; set; }
}
