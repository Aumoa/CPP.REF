// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Locales;

public interface IErrors
{
    string AssemblyLocationNotFound { get; }

    string InvalidEngineDirectory { get; }

    string TargetRuleConstructorNotFound { get; }

    string ModuleRuleConstructorNotFound { get; }

    string DependencyModuleNotFound { get; }

    string DuplicatedModuleDefinitions { get; }
}
