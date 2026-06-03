using System.Runtime.CompilerServices;
using System.Security;
using System.Text;
using System.Text.Encodings.Web;
using System.Text.Json;
using System.Text.Json.Nodes;
using System.Text.Json.Serialization;

namespace AylaEngine;

internal static class VSCCppProjectGenerator
{
    private record Configuration
    {
        [JsonPropertyName("name")]
        public required string Name { get; set; }

        [JsonPropertyName("includePath")]
        public required string[] IncludePath { get; set; }

        [JsonPropertyName("defines")]
        public required string[] Defines { get; set; }

        [JsonPropertyName("compilerPath")]
        public required string CompilerPath { get; set; }

        [JsonPropertyName("cStandard")]
        public required string CStandard { get; set; }

        [JsonPropertyName("cppStandard")]
        public required string CppStandard { get; set; }

        [JsonPropertyName("intelliSenseMode")]
        public required string IntelliSenseMode { get; set; }
    }

    private record Group
    {
        [JsonPropertyName("kind")]
        public required string Kind { get; set; }

        [JsonPropertyName("isDefault")]
        public required bool IsDefault { get; set; }
    }

    private record Task
    {
        [JsonPropertyName("label")]
        public required string Label { get; set; }

        [JsonPropertyName("type")]
        public required string Type { get; set; }

        [JsonPropertyName("command")]
        public required string Command { get; set; }

        [JsonPropertyName("args")]
        public required string[] Arguments { get; set; }

        [JsonPropertyName("group")]
        public required Group Group { get; set; }

        [JsonPropertyName("problemMatcher")]
        public required string[] ProblemMatcher { get; set; }
    }

    private record LaunchConfigurationEnvironment
    {
        [JsonPropertyName("name")]
        public required string Name { get; set; }

        [JsonPropertyName("value")]
        public required string Value { get; set; }
    }

    private record LaunchConfigurationSetupCommand
    {
        [JsonPropertyName("description")]
        public string? Description { get; set; }

        [JsonPropertyName("text")]
        public required string Text { get; set; }

        [JsonPropertyName("ignoreFailures")]
        public bool IgnoreFailures { get; set; } = false;
    }

    private record LaunchConfiguration
    {
        [JsonPropertyName("name")]
        public required string Name { get; set; }

        [JsonPropertyName("type")]
        public required string Type { get; set; }

        [JsonPropertyName("request")]
        public required string Request { get; set; }

        [JsonPropertyName("program")]
        public required string Program { get; set; }

        [JsonPropertyName("args")]
        public string[] Arguments { get; set; } = [];

        [JsonPropertyName("stopAtEntry")]
        public bool StopAtEntry { get; set; } = false;

        [JsonPropertyName("cwd")]
        public string? WorkingDirectory { get; set; }

        [JsonPropertyName("environment")]
        public LaunchConfigurationEnvironment[] Environment { get; set; } = [];

        [JsonPropertyName("externalConsole")]
        public bool ExternalConsole { get; set; } = false;

        [JsonPropertyName("MIMode")]
        public string MIMode { get; set; } = "gdb";

        [JsonPropertyName("setupCommands")]
        public LaunchConfigurationSetupCommand[] SetupCommands { get; set; } = [
            new LaunchConfigurationSetupCommand
            {
                Description = "Enable pretty-printing for gdb",
                Text = "-enable-pretty-printing",
                IgnoreFailures = true
            }
        ];

        [JsonPropertyName("miDebuggerPath")]
        public string MIDebuggerPath { get; set; } = "/usr/bin/gdb";

        [JsonPropertyName("preLaunchTask")]
        public string? PreLaunchTask { get; set; }
    }

    private static readonly JsonSerializerOptions JsonOptions = new()
    {
        Encoder = JavaScriptEncoder.UnsafeRelaxedJsonEscaping,
        WriteIndented = true
    };

    public static async ValueTask GenerateAsync(Solution solution, ModuleRulesResolverFactory resolverFactory, ModuleProject project, List<string> outputFolders, CancellationToken cancellationToken)
    {
        string vscode_FullName = Path.Combine(project.SourceDirectory, ".vscode");
        string c_cpp_properties_FileName = "c_cpp_properties.json";
        string c_cpp_properties_FullName = Path.Combine(vscode_FullName, c_cpp_properties_FileName);
        string tasks_FileName = "tasks.json";
        string tasks_FullName = Path.Combine(vscode_FullName, tasks_FileName);
        string launch_FileName = "launch.json";
        string launch_FullName = Path.Combine(vscode_FullName, launch_FileName);
        lock (outputFolders)
        {
            outputFolders.Add(project.SourceDirectory);
        }

        var currentAssemblyLocation = Global.AssemblyLocation;

        var installation = Installation.CreateDefaultInstallation();
        List<Configuration> configurations = [];
        List<Task> tasks = [];
        List<LaunchConfiguration> launchConfigurations = [];
        foreach (var targetInfo in TargetInfo.GetAllTargets())
        {
            if (targetInfo.Platform == PlatformInfo.Current)
            {
                var resolver = resolverFactory.GetResolver(project, targetInfo);
                string compilerPath = await installation.GetCompilerPath(targetInfo, cancellationToken);
                string intelliSenseMode = await installation.GetIntelliSenseMode(targetInfo, cancellationToken);
                var rule = resolver.Rules;
                var outputFileName = project.Group.OutputFileName(installation, targetInfo, project.Name, rule.Type, FolderPolicy.PathType.Linux);
                var fileName = Path.GetFileName(outputFileName);
                if (fileName.StartsWith("lib") && fileName.EndsWith(".so"))
                {
                    fileName = fileName[3..^3];
                }

                outputFileName = Path.Combine(Path.GetDirectoryName(outputFileName)!, fileName);

                configurations.Add(new Configuration
                {
                    Name = FormatTargetName(targetInfo),
                    IncludePath = resolver.IncludePaths.ToArray(),
                    Defines = AppendPlatformMacros(resolver.AdditionalMacros).Select(FormatMacro).ToArray(),
                    CompilerPath = compilerPath,
                    CStandard = "c11",
                    CppStandard = "c++23",
                    IntelliSenseMode = intelliSenseMode
                });

                string[] projectArgs = solution.ProjectFile == null ? [] : ["--project", solution.ProjectFile];

                tasks.Add(new Task
                {
                    Label = project.Name + " Build " + FormatTargetName(targetInfo),
                    Type = "shell",
                    Command = "dotnet",
                    Arguments = [
                        currentAssemblyLocation, "build",
                        .. projectArgs,
                        "--target", project.Name,
                        "--config", targetInfo.Config.ToString(),
                        "--generator", "VisualStudioCode",
                        targetInfo.Editor ? "--editor" : string.Empty
                    ],
                    Group = new()
                    {
                        Kind = "build",
                        IsDefault = false
                    },
                    ProblemMatcher = [
                        "$gcc"
                    ]
                });

                tasks.Add(new Task
                {
                    Label = project.Name + " Clean " + FormatTargetName(targetInfo),
                    Type = "shell",
                    Command = "dotnet",
                    Arguments = [
                        currentAssemblyLocation, "build",
                        .. projectArgs,
                        "--target", project.Name,
                        "--config", targetInfo.Config.ToString(),
                        "--generator", "VisualStudioCode",
                        targetInfo.Editor ? "--editor" : string.Empty,
                        "--clean", "CleanOnly"
                    ],
                    Group = new()
                    {
                        Kind = "clean",
                        IsDefault = false
                    },
                    ProblemMatcher = [
                        "$gcc"
                    ]
                });

                tasks.Add(new Task
                {
                    Label = project.Name + " Generate " + FormatTargetName(targetInfo),
                    Type = "shell",
                    Command = "dotnet",
                    Arguments = [
                        currentAssemblyLocation, "build",
                        .. projectArgs,
                        "--target", project.Name,
                        "--config", targetInfo.Config.ToString(),
                        "--generator", "VisualStudioCode",
                        targetInfo.Editor ? "--editor" : string.Empty,
                        "--clean", "GenerateOnly"
                    ],
                    Group = new()
                    {
                        Kind = "generate",
                        IsDefault = false
                    },
                    ProblemMatcher = [
                        "$gcc"
                    ]
                });

                string[] args = [];
                if (rule.Type == ModuleType.Game)
                {
                    args = ["--gameassembly", outputFileName];
                }

                launchConfigurations.Add(new LaunchConfiguration
                {
                    Name = FormatTargetName(targetInfo),
                    Type = "cppdbg",
                    Request = "launch",
                    Program = Path.Combine(solution.EngineGroup.Output(targetInfo, FolderPolicy.PathType.Linux)) + "/Launch",
                    Arguments = [.. args],
                    StopAtEntry = false,
                    WorkingDirectory = Path.Combine(solution.EngineGroup.Output(targetInfo, FolderPolicy.PathType.Linux)),
                    Environment = [
                        new LaunchConfigurationEnvironment
                        {
                            Name = "LD_LIBRARY_PATH",
                            Value = string.Join(':', ["/usr/local/gcc-15/lib64", "$LD_LIBRARY_PATH"])
                        }
                    ],
                    MIMode = "gdb",
                    MIDebuggerPath = "/usr/bin/gdb",
                    PreLaunchTask = project.Name + " Build " + FormatTargetName(targetInfo)
                });
            }

            IEnumerable<MacroSet> AppendPlatformMacros(IEnumerable<MacroSet> set)
            {
                switch (targetInfo.Platform.Group)
                {
                    case PlatformGroup.Windows:
                        set = set.Concat(["PLATFORM_WINDOWS=1", "_UNICODE", "UNICODE"]);
                        break;
                    case PlatformGroup.Linux:
                        set = set.Append("PLATFORM_LINUX=1");
                        break;
                    case PlatformGroup.OSX:
                        set = set.Append("PLATFORM_OSX=1");
                        break;
                }

                if (targetInfo.Config != AylaEngine.Configuration.Shipping)
                {
                    set = set.Append("DO_CHECK=1");
                }

                return set;
            }
        }

        Directory.CreateDirectory(vscode_FullName);

        {
            const int version = 4;
            string c_cpp_properties_Json = JsonSerializer.Serialize(new
            {
                configurations,
                version
            }, JsonOptions);

            await File.WriteAllTextAsync(c_cpp_properties_FullName, c_cpp_properties_Json, cancellationToken);
        }

        {
            const string version = "2.0.0";
            string tasks_Json = JsonSerializer.Serialize(new
            {
                version,
                tasks
            }, JsonOptions);

            await File.WriteAllTextAsync(tasks_FullName, tasks_Json, cancellationToken);
        }

        {
            const string version = "0.2.0";
            string launch_Json = JsonSerializer.Serialize(new
            {
                version,
                configurations = launchConfigurations
            }, JsonOptions);

            await File.WriteAllTextAsync(launch_FullName, launch_Json, cancellationToken);
        }

        static string FormatMacro(MacroSet set)
        {
            if (set.Value == null)
            {
                return set.VarName;
            }
            else
            {
                return $"{set.VarName}={set.Value}";
            }
        }

        static string FormatTargetName(TargetInfo info)
        {
            string name = info.Config.ToString();
            if (info.Editor)
            {
                name += " Editor";
            }

            return name;
        }
    }
}
