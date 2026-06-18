namespace AylaEngine;

internal static partial class BuildRunner
{
    internal class ShaderCompileTask : ITask
    {
        private const int MaxShaderTypeDetectionLines = 20; // Maximum lines to scan for shader type annotation

        private readonly ModuleProject m_Project;
        private readonly TargetInfo m_TargetInfo;
        private readonly SourceCodeDescriptor[] m_ShaderFiles;
        private readonly GroupDescriptor m_EngineGroup;
        private readonly ModuleRulesResolverFactory m_ResolverFactory;
        private readonly TaskCompletionSource m_CompletionSource = new();

        public ShaderCompileTask(ModuleProject project, TargetInfo targetInfo, SourceCodeDescriptor[] shaderFiles, GroupDescriptor engineGroup, ModuleRulesResolverFactory resolverFactory)
        {
            m_Project = project;
            m_TargetInfo = targetInfo;
            m_ShaderFiles = shaderFiles;
            m_EngineGroup = engineGroup;
            m_ResolverFactory = resolverFactory;
        }

        public async Task<Terminal.Output> CompileAsync(IList<ModuleTask> moduleTasks, Installation installation, CancellationToken cancellationToken)
        {
            try
            {
                if (m_ShaderFiles.Length == 0)
                {
                    m_CompletionSource.SetResult();
                    return Terminal.Output.Success("Shader Compiler", "No shaders to compile");
                }

                var resolver = m_ResolverFactory.GetResolver(m_Project, m_TargetInfo);
                var intDir = resolver.Group.Intermediate(resolver.Name, m_TargetInfo, FolderPolicy.PathType.Current);
                var outDir = resolver.Group.Output(m_TargetInfo, FolderPolicy.PathType.Current);

                // Ensure directories exist
                Directory.CreateDirectory(intDir);
                Directory.CreateDirectory(outDir);

                // Support vullkan
                var vulkanSdk = Environment.GetEnvironmentVariable("VULKAN_SDK");
                bool supportVulkan = !string.IsNullOrEmpty(vulkanSdk);

                // Generate shader makefile
                var makefilePath = Path.Combine(intDir, "ShaderCompilation.txt");
                var needsCompilation = await GenerateMakefileAsync(makefilePath, intDir, outDir, supportVulkan, cancellationToken);

                if (!needsCompilation.Any())
                {
                    // No shaders need compilation
                    m_CompletionSource.SetResult();
                    return Terminal.Output.Success("Shader Compiler", "All shaders up to date");
                }

                var workerTargetInfo = new TargetInfo { Platform = m_TargetInfo.Platform, Config = Configuration.Development, Editor = false };
                var workerPath = Path.Combine(m_EngineGroup.Output(workerTargetInfo, FolderPolicy.PathType.Current), PlatformUtility.GetExecutableFileName("ShaderCompileWorker"));
                if (!File.Exists(workerPath))
                {
                    Console.Error.WriteLine("Error: Shader compile worker executable not found at {0}", workerPath);
                    throw TerminateException.Abort();
                }

                // Run DXC
                string path = Environment.GetEnvironmentVariable("PATH") ?? "";
                if (supportVulkan)
                {
                    path = $"{Path.Combine(vulkanSdk!, "Bin")};{path}";
                }

                var options = new Terminal.Options
                {
                    Executable = workerPath,
                    WorkingDirectory = Path.GetDirectoryName(workerPath)!,
                    Logging = Terminal.Logging.All,
                    Environments = [new KeyValuePair<string, string>("PATH", path)]
                };

                var dxcOutput = await Terminal.ExecuteCommandAsync($"\"{makefilePath}\"", options, cancellationToken);
                if (dxcOutput.ExitCode != 0)
                {
                    throw new TerminalExecutionException(dxcOutput);
                }

                m_CompletionSource.SetResult();
                return dxcOutput;
            }
            catch (OperationCanceledException)
            {
                m_CompletionSource.SetCanceled();
                throw;
            }
            catch (Exception e)
            {
                m_CompletionSource.SetException(e);
                throw;
            }
        }

        private async Task<List<SourceCodeDescriptor>> GenerateMakefileAsync(string makefilePath, string intDir, string outDir, bool supportVulkan, CancellationToken cancellationToken)
        {
            var needsCompilation = new List<SourceCodeDescriptor>();
            var lines = new List<string>();
            
            lines.Add("# Auto-generated shader compilation makefile");
            lines.Add("# Format: <source> -t <type> -e <entrypoint> [--vulkan] -o <output> -d <deps> [-I <include_path>]...");
            lines.Add("");

            foreach (var shaderFile in m_ShaderFiles)
            {
                if (shaderFile.Type != SourceCodeType.HLSLShader)
                {
                    continue;
                }

                // Detect shader type from file content
                var shaderType = await DetectShaderTypeAsync(shaderFile.FilePath, cancellationToken);
                if (shaderType == null)
                {
                    continue; // Skip files without shader type annotation
                }

                var fileName = Path.GetFileNameWithoutExtension(shaderFile.FilePath);
                var outputBasePath = Path.Combine(outDir, "Shaders", fileName);
                var includePaths = GetIncludePaths(shaderFile);
                var includeArgs = string.Join(" ", includePaths.Select(p => $"-I \"{p}\""));

                var shaderNeedsCompilation = false;
                var csoFile = outputBasePath + ".cso";
                var csoDepsFile = Path.Combine(intDir, fileName + ".cso.deps");

                if (await NeedsCompilationAsync(csoFile, csoDepsFile, cancellationToken))
                {
                    shaderNeedsCompilation = true;

                    lines.Add($"\"{shaderFile.FilePath}\" -t {shaderType} -e main -o \"{outputBasePath}\" -d \"{csoDepsFile}\" {includeArgs}");
                }

                if (supportVulkan)
                {
                    var spvFile = outputBasePath + ".spv";
                    var spvDepsFile = Path.Combine(intDir, fileName + ".spv.deps");

                    if (await NeedsCompilationAsync(spvFile, spvDepsFile, cancellationToken))
                    {
                        shaderNeedsCompilation = true;

                        lines.Add($"\"{shaderFile.FilePath}\" -t {shaderType} -e main --vulkan -o \"{outputBasePath}\" -d \"{spvDepsFile}\" {includeArgs}");
                    }
                }

                if (shaderNeedsCompilation)
                {
                    needsCompilation.Add(shaderFile);
                    lines.Add("");
                }
            }

            await File.WriteAllTextAsync(makefilePath, string.Join(Environment.NewLine, lines), cancellationToken);
            return needsCompilation;
        }

        private async Task<string?> DetectShaderTypeAsync(string filePath, CancellationToken cancellationToken)
        {
            // Read first few lines to detect shader type from comment
            var lines = await File.ReadAllLinesAsync(filePath, cancellationToken);
            foreach (var line in lines.Take(MaxShaderTypeDetectionLines))
            {
                var trimmed = line.Trim();
                
                // Look for shader type in comments
                if (trimmed.StartsWith("//") || trimmed.StartsWith("/*"))
                {
                    var comment = trimmed.TrimStart('/', '*').Trim().ToLower();
                    
                    if (comment.Contains("shader:") || comment.Contains("type:") || comment.Contains("shader type:"))
                    {
                        if (comment.Contains("vertex") || comment.Contains("vs"))
                            return "vertex";
                        if (comment.Contains("pixel") || comment.Contains("ps") || comment.Contains("fragment"))
                            return "pixel";
                        if (comment.Contains("compute") || comment.Contains("cs"))
                            return "compute";
                        if (comment.Contains("library") || comment.Contains("lib") || comment.Contains("raytracing") || comment.Contains("rt"))
                            return "library";
                    }
                }
                
                // Also check for HLSL shader attributes
                if (trimmed.Contains("[shader("))
                {
                    if (trimmed.Contains("\"vertex\""))
                        return "vertex";
                    if (trimmed.Contains("\"pixel\""))
                        return "pixel";
                    if (trimmed.Contains("\"compute\""))
                        return "compute";
                    if (trimmed.Contains("\"raygeneration\"") || trimmed.Contains("\"closesthit\"") || 
                        trimmed.Contains("\"anyhit\"") || trimmed.Contains("\"miss\"") || 
                        trimmed.Contains("\"callable\"") || trimmed.Contains("\"intersection\""))
                        return "library";
                }
                
                // Stop searching after non-comment/non-empty lines
                if (!string.IsNullOrWhiteSpace(trimmed) && 
                    !trimmed.StartsWith("//") && 
                    !trimmed.StartsWith("/*") && 
                    !trimmed.StartsWith("*") &&
                    !trimmed.StartsWith("#"))
                {
                    break;
                }
            }

            return null;
        }

        private async Task<bool> NeedsCompilationAsync(string outputFile, string depsFile, CancellationToken cancellationToken)
        {
            if (!File.Exists(outputFile))
            {
                return true;
            }

            // If deps file doesn't exist, need compilation
            if (!File.Exists(depsFile))
            {
                return true;
            }

            try
            {
                // Read dependency file
                var depsContent = await File.ReadAllTextAsync(depsFile, cancellationToken);
                var outputTime = File.GetLastWriteTimeUtc(outputFile);
                
                // Parse dependencies (format: "output: \ dep1 \ dep2 ...")
                var lines = depsContent.Split('\n');
                foreach (var line in lines)
                {
                    var trimmed = line.Trim().TrimEnd('\\').Trim();
                    if (string.IsNullOrWhiteSpace(trimmed) || trimmed.EndsWith(':'))
                    {
                        continue;
                    }

                    if (File.Exists(trimmed))
                    {
                        var depTime = File.GetLastWriteTimeUtc(trimmed);
                        if (depTime > outputTime)
                        {
                            return true; // Dependency is newer
                        }
                    }
                }

                return false; // All dependencies are older than output
            }
            catch (IOException ex)
            {
                // If we can't read the dependency file due to IO error, assume we need to recompile
                Console.WriteLine($"Warning: Could not read dependency file {depsFile}: {ex.Message}");
                return true;
            }
            catch (UnauthorizedAccessException ex)
            {
                // If we can't access the file, assume we need to recompile
                Console.WriteLine($"Warning: Access denied to dependency file {depsFile}: {ex.Message}");
                return true;
            }
        }

        private List<string> GetIncludePaths(SourceCodeDescriptor shaderFile)
        {
            var includePaths = new List<string>();
            
            // Add the shader's directory as an include path
            includePaths.Add(shaderFile.Directory);
            
            // Add common include paths
            var resolver = m_ResolverFactory.GetResolver(m_Project, m_TargetInfo);
            foreach (var includePath in resolver.IncludePaths)
            {
                var fullPath = Path.Combine(m_Project.SourceDirectory, includePath);
                if (Directory.Exists(fullPath))
                {
                    includePaths.Add(fullPath);
                }
            }

            return includePaths;
        }

        public Task Task => m_CompletionSource.Task;

        public GroupDescriptor Group => m_Project.Group;
    }
}
