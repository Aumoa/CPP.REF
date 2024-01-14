// Copyright 2020-2023 Aumoa.lib. All right reserved.

using AE.Projects;

namespace AE.CompilerServices.Makefiles;

public class MakefileLink : MakefileNode
{
    public required ModuleInformation ModuleInfo { get; init; }

    public required bool IsRequired { get; init; }
}
