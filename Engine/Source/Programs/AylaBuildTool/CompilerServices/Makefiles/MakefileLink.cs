// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public class MakefileLink : MakefileNode
{
    public required ModuleInformation ModuleInfo { get; init; }

    public required bool IsRequired { get; init; }
}
