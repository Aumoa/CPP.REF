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

    string InvalidProjectFormat { get; }

    string StartCMakeError { get; }

    string CMakeReturnCodeError { get; }

    string MultipleModuleNotSuportedError { get; }

    string NotSupportedBuildHostPlatform { get; }

    string CompilerNotSupported { get; }

    string InvalidToolChainInstallation { get; }

    string TargetNotFoundException { get; }

    string NotSupportedArchitecture { get; }

    string NullReferenceInRule { get; }

    string NotSupportedType { get; }

    string ModuleRuleDuplicated { get; }

    string ModuleRuleNotFound { get; }

    string TargetRuleNotFound { get; }

    string RuleClassDefinitions { get; }

    string TargetRuleDuplicated { get; }

    string InvalidModuleFormat { get; }
}
