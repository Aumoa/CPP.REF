using System.Runtime.CompilerServices;
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

        [JsonPropertyName("shell")]
        public required string Shell { get; set; }

        [JsonPropertyName("command")]
        public required string Command { get; set; }

        [JsonPropertyName("args")]
        public required string[] Arguments { get; set; }

        [JsonPropertyName("group")]
        public required Group Group { get; set; }
    }

    private static readonly JsonSerializerOptions JsonOptions = new()
    {
        Encoder = JavaScriptEncoder.UnsafeRelaxedJsonEscaping,
        WriteIndented = true
    };

    public static async ValueTask GenerateAsync(Solution solution, ModuleProject project, CancellationToken cancellationToken)
    {
        string vscode_FullName = Path.Combine(project.SourceDirectory, ".vscode");
        string c_cpp_properties_FileName = "c_cpp_properties.json";
        string c_cpp_properties_FullName = Path.Combine(vscode_FullName, c_cpp_properties_FileName);
        string tasks_FileName = "tasks.json";
        string tasks_FullName = Path.Combine(vscode_FullName, tasks_FileName);

        var installation = Installation.CreateDefaultInstallation();
        List<Configuration> configurations = [];
        foreach (var targetInfo in TargetInfo.GetAllTargets())
        {
            if (targetInfo.Platform == PlatformInfo.Current)
            {
                var resolver = project.GetResolver(targetInfo);
                string compilerPath = await installation.GetCompilerPath(targetInfo, cancellationToken);
                string intelliSenseMode = await installation.GetIntelliSenseMode(targetInfo, cancellationToken);

                configurations.Add(new Configuration
                {
                    Name = FormatTargetName(targetInfo),
                    IncludePath = resolver.IncludePaths.ToArray(),
                    Defines = AppendPlatformMacros(resolver.AdditionalMacros).Select(FormatMacro).ToArray(),
                    CompilerPath = compilerPath,
                    CStandard = "c11",
                    CppStandard = "c++20",
                    IntelliSenseMode = intelliSenseMode
                });
            }

            IEnumerable<MacroSet> AppendPlatformMacros(IEnumerable<MacroSet> set)
            {
                switch (targetInfo.Platform.Group)
                {
                    case PlatformGroup.Windows:
                        return set.Concat(["PLATFORM_WINDOWS=1", "_UNICODE", "UNICODE"]);
                    case PlatformGroup.Linux:
                        return set.Append("PLATFORM_LINUX=1");
                }

                return set;
            }
        }

        const int version = 4;
        string json = JsonSerializer.Serialize(new
        {
            configurations,
            version
        }, JsonOptions);

        Directory.CreateDirectory(vscode_FullName);
        await File.WriteAllTextAsync(c_cpp_properties_FullName, json, cancellationToken);

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
