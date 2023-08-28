// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Exceptions;

public enum KnownErrorCode
{
    NotSupportedType = 1,
    ModuleDuplicate = 2,
    MultipleClassDefinitions = 3,
    ConstructorNotFound = 4,
    TargetDuplicate = 5,
    TargetNotFound = 6,
    DependencyModuleNotFound = 7,
    ModuleNotFound = 8,
    CompileError = 9,
    InvalidModule = 10,
    PlatformCompilerNotFound = 11,
    NotSupportedCompiler = 12,
    NotSupportedBuildHostPlatform = 13
}
