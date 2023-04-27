// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Locales;

public interface IErrors
{
    string AssemblyLocationNotFound { get; }

    string InvalidEngineDirectory { get; }

    string TargetRuleConstructorNotFound(string RuleClassName);

    string ModuleRuleConstructorNotFound(string ClassName);

    string DependencyModuleNotFound(string ModuleName, string RequiredModule);

    string DuplicatedModuleDefinitions { get; }

    string InvalidProjectFormat { get; }

    string StartCMakeError { get; }

    string CMakeReturnCodeError(int ReturnCode);

    string MultipleModuleNotSuportedError { get; }

    string NotSupportedBuildHostPlatform { get; }

    string CompilerNotSupported { get; }

    string InvalidToolChainInstallation { get; }

    string TargetNotFoundException(string TargetName);

    string NotSupportedArchitecture(string ArchitectureName);

    string NullReferenceInRule(string PropertyName, string OwnedType);

    string NotSupportedType(string PropertyName, string OwnedType);

    string ModuleRuleDuplicated(string DirectoryName);

    string ModuleRuleNotFound(string DirectoryName);

    string TargetRuleNotFound(string DirectoryName, string RuleName);

    string RuleClassDefinitions(string RuleName);

    string TargetRuleDuplicated { get; }

    string InvalidModuleFormat(string FileName);

    string PlatformCompilerNotFound(string PlatformName);

    string NotSupportedCompiler { get; }
}
