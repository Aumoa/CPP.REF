// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace BuildTool;

public partial class VSGenerator
{
    struct BuildConfiguration
    {
        public string Name;
        public string Configuration;
        public string Optimization;
        public string BasicRuntimeChecks;
        public string Macros;
        public string DebugInformationFormat;
        public string RuntimeLibrary;
        public string ExceptionHandling;
        public bool bUseDebugLibrary;
        public bool bWholeProgramOptimization;
        public bool bLinkIncremental;
        public bool bFunctionLevelLinking;
        public bool bIntrinsicFunctions;
        public bool bEnableCOMDATFolding;
        public bool bOptimizeReferences;
    }

    struct BuildTarget
    {
        public BuildConfiguration Game;
        public BuildConfiguration ThirdParty;
        public BuildConfiguration Engine;

        public BuildConfiguration this[ModuleCategory Index]
        {
            get
            {
                return Index switch
                {
                    ModuleCategory.Game => Game,
                    ModuleCategory.ThirdParty => ThirdParty,
                    ModuleCategory.Engine => Engine,
                    _ => throw new ArgumentException(Index.ToString()),
                };
            }
        }
    }

    static VSGenerator()
    {
        Debug = new BuildTarget
        {
            Game = new BuildConfiguration
            {
                Name = "Debug",
                Configuration = "Debug",
                Optimization = "Disabled",
                BasicRuntimeChecks = "EnableFastChecks",
                Macros = "DO_CHECK=1;SHIPPING=0",
                DebugInformationFormat = "EditAndContinue",
                RuntimeLibrary = "MultiThreadedDebug",
                ExceptionHandling = "Async",
                bUseDebugLibrary = true,
                bWholeProgramOptimization = false,
                bLinkIncremental = true,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = false,
                bEnableCOMDATFolding = false,
                bOptimizeReferences = false
            },
            Engine = new BuildConfiguration
            {
                Name = "Debug",
                Configuration = "Debug",
                Optimization = "Disabled",
                BasicRuntimeChecks = "EnableFastChecks",
                Macros = "DO_CHECK=1;SHIPPING=0",
                DebugInformationFormat = "EditAndContinue",
                RuntimeLibrary = "MultiThreadedDebug",
                ExceptionHandling = "Async",
                bUseDebugLibrary = true,
                bWholeProgramOptimization = false,
                bLinkIncremental = true,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = false,
                bEnableCOMDATFolding = false,
                bOptimizeReferences = false
            },
            ThirdParty = new BuildConfiguration
            {
                Name = "Debug",
                Configuration = "Debug",
                Optimization = "MaxSpeed",
                BasicRuntimeChecks = "Default",
                Macros = "DO_CHECK=0;SHIPPING=0",
                DebugInformationFormat = "ProgramDatabase",
                RuntimeLibrary = "MultiThreaded",
                ExceptionHandling = "Async",
                bUseDebugLibrary = true,
                bWholeProgramOptimization = true,
                bLinkIncremental = false,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = true,
                bEnableCOMDATFolding = true,
                bOptimizeReferences = true
            }
        };

        DebugGame = new BuildTarget
        {
            Game = new BuildConfiguration
            {
                Name = "DebugGame",
                Configuration = "DebugGame",
                Optimization = "Disabled",
                BasicRuntimeChecks = "EnableFastChecks",
                Macros = "DO_CHECK=1;SHIPPING=0",
                DebugInformationFormat = "EditAndContinue",
                RuntimeLibrary = "MultiThreadedDebug",
                ExceptionHandling = "Async",
                bUseDebugLibrary = true,
                bWholeProgramOptimization = false,
                bLinkIncremental = true,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = false,
                bEnableCOMDATFolding = false,
                bOptimizeReferences = false
            },
            Engine = new BuildConfiguration
            {
                Name = "DebugGame",
                Configuration = "DebugGame",
                Optimization = "MaxSpeed",
                BasicRuntimeChecks = "Default",
                Macros = "DO_CHECK=1;SHIPPING=0",
                DebugInformationFormat = "ProgramDatabase",
                RuntimeLibrary = "MultiThreadedDebug",
                ExceptionHandling = "Async",
                bUseDebugLibrary = true,
                bWholeProgramOptimization = true,
                bLinkIncremental = false,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = true,
                bEnableCOMDATFolding = true,
                bOptimizeReferences = true
            },
            ThirdParty = new BuildConfiguration
            {
                Name = "DebugGame",
                Configuration = "DebugGame",
                Optimization = "MaxSpeed",
                BasicRuntimeChecks = "Default",
                Macros = "DO_CHECK=0;SHIPPING=0",
                DebugInformationFormat = "ProgramDatabase",
                RuntimeLibrary = "MultiThreadedDebug",
                ExceptionHandling = "Async",
                bUseDebugLibrary = true,
                bWholeProgramOptimization = true,
                bLinkIncremental = false,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = true,
                bEnableCOMDATFolding = true,
                bOptimizeReferences = true
            }
        };

        Development = new BuildTarget
        {
            Game = new BuildConfiguration
            {
                Name = "Development",
                Configuration = "Development",
                Optimization = "MaxSpeed",
                BasicRuntimeChecks = "Default",
                Macros = "DO_CHECK=1;SHIPPING=0",
                DebugInformationFormat = "ProgramDatabase",
                RuntimeLibrary = "MultiThreaded",
                ExceptionHandling = "Async",
                bUseDebugLibrary = false,
                bWholeProgramOptimization = true,
                bLinkIncremental = false,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = true,
                bEnableCOMDATFolding = true,
                bOptimizeReferences = true
            },
            Engine = new BuildConfiguration
            {
                Name = "Development",
                Configuration = "Development",
                Optimization = "MaxSpeed",
                BasicRuntimeChecks = "Default",
                Macros = "DO_CHECK=1;SHIPPING=0;",
                DebugInformationFormat = "ProgramDatabase",
                RuntimeLibrary = "MultiThreaded",
                ExceptionHandling = "Async",
                bUseDebugLibrary = false,
                bWholeProgramOptimization = true,
                bLinkIncremental = false,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = true,
                bEnableCOMDATFolding = true,
                bOptimizeReferences = true
            },
            ThirdParty = new BuildConfiguration
            {
                Name = "Development",
                Configuration = "Development",
                Optimization = "MaxSpeed",
                BasicRuntimeChecks = "Default",
                Macros = "DO_CHECK=1;SHIPPING=0;",
                DebugInformationFormat = "ProgramDatabase",
                RuntimeLibrary = "MultiThreaded",
                ExceptionHandling = "Async",
                bUseDebugLibrary = false,
                bWholeProgramOptimization = true,
                bLinkIncremental = false,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = true,
                bEnableCOMDATFolding = true,
                bOptimizeReferences = true
            }
        };

        Shipping = new BuildTarget
        {
            Game = new BuildConfiguration
            {
                Name = "Shipping",
                Configuration = "Shipping",
                Optimization = "MaxSpeed",
                BasicRuntimeChecks = "Default",
                Macros = "DO_CHECK=0;SHIPPING=1",
                DebugInformationFormat = "ProgramDatabase",
                RuntimeLibrary = "MultiThreaded",
                ExceptionHandling = "Sync",
                bUseDebugLibrary = false,
                bWholeProgramOptimization = true,
                bLinkIncremental = false,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = true,
                bEnableCOMDATFolding = true,
                bOptimizeReferences = true
            },
            Engine = new BuildConfiguration
            {
                Name = "Shipping",
                Configuration = "Shipping",
                Optimization = "MaxSpeed",
                BasicRuntimeChecks = "Default",
                Macros = "DO_CHECK=0;SHIPPING=1",
                DebugInformationFormat = "ProgramDatabase",
                RuntimeLibrary = "MultiThreaded",
                ExceptionHandling = "Sync",
                bUseDebugLibrary = false,
                bWholeProgramOptimization = true,
                bLinkIncremental = false,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = true,
                bEnableCOMDATFolding = true,
                bOptimizeReferences = true
            },
            ThirdParty = new BuildConfiguration
            {
                Name = "Shipping",
                Configuration = "Shipping",
                Optimization = "MaxSpeed",
                BasicRuntimeChecks = "Default",
                Macros = "DO_CHECK=0;SHIPPING=1",
                DebugInformationFormat = "ProgramDatabase",
                RuntimeLibrary = "MultiThreaded",
                ExceptionHandling = "Sync",
                bUseDebugLibrary = false,
                bWholeProgramOptimization = true,
                bLinkIncremental = false,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = true,
                bEnableCOMDATFolding = true,
                bOptimizeReferences = true
            }
        };
    }
}
