// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public interface IErrors
{
    string AssemblyLocationNotFound { get; }

    string InvalidEngineDirectory { get; }

    string TargetRuleConstructorNotFound(string ruleClassName);

    string ModuleRuleConstructorNotFound(string className);

    string DependencyModuleNotFound(string moduleName, string requiredModule);

    string ModuleNotFound(string moduleName);

    string DuplicatedModuleDefinitions { get; }

    string InvalidProjectFormat { get; }

    string StartCMakeError { get; }

    string CMakeReturnCodeError(int returnCode);

    string MultipleModuleNotSuportedError { get; }

    string NotSupportedBuildHostPlatform { get; }

    string CompilerNotSupported { get; }

    string InvalidToolChainInstallation { get; }

    string TargetNotFoundException(string targetName);

    string NotSupportedArchitecture(string architectureName);

    string NullReferenceInRule(string propertyName, string ownedType);

    string NotSupportedType(string propertyName, string ownedType);

    string ModuleRuleDuplicated(string directoryName);

    string ModuleRuleNotFound(string directoryName);

    string TargetRuleNotFound(string directoryName, string ruleName);

    string RuleClassDefinitions(string ruleName);

    string TargetRuleDuplicated { get; }

    string InvalidModuleFormat(string fileName);

    string PlatformCompilerNotFound(string platformName);

    string NotSupportedCompiler { get; }
}
