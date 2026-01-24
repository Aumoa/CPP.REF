namespace AylaEngine;

internal static partial class BuildRunner
{
    private class ShaderCompileTask : ITask
    {
        private const int MaxShaderTypeDetectionLines = 20; // Maximum lines to scan for shader type annotation
        
        private readonly ModuleProject m_Project;
        private readonly TargetInfo m_TargetInfo;
        private readonly SourceCodeDescriptor[] m_ShaderFiles;
        private readonly TaskCompletionSource m_CompletionSource = new();

        public ShaderCompileTask(ModuleProject project, TargetInfo targetInfo, SourceCodeDescriptor[] shaderFiles)
        {
            m_Project = project;
            m_TargetInfo = targetInfo;
            m_ShaderFiles = shaderFiles;
        }

        public async Task<Terminal.Output> CompileAsync(Installation installation, CancellationToken cancellationToken)
        {
            try
            {
                if (m_ShaderFiles.Length == 0)
                {
                    m_CompletionSource.SetResult();
                    return Terminal.Output.Success("Shader Compiler", "No shaders to compile");
                }

                var resolver = m_Project.GetResolver(m_TargetInfo);
                var intDir = resolver.Group.Intermediate(resolver.Name, m_TargetInfo, FolderPolicy.PathType.Current);
                var outDir = resolver.Group.Output(m_TargetInfo, FolderPolicy.PathType.Current);

                // Ensure directories exist
                Directory.CreateDirectory(intDir);
                Directory.CreateDirectory(outDir);

                // Generate shader makefile
                var makefilePath = Path.Combine(intDir, "ShaderCompilation.txt");
                var needsCompilation = await GenerateMakefileAsync(makefilePath, intDir, outDir, cancellationToken);

                if (!needsCompilation.Any())
                {
                    // No shaders need compilation
                    m_CompletionSource.SetResult();
                    return Terminal.Output.Success("Shader Compiler", "All shaders up to date");
                }

                // Invoke DXC compiler
                var targetInfo = TargetInfo.Environment;
                var dxcPath = Path.Combine(resolver.Group.BinariesDirectory, targetInfo.Platform.Name, targetInfo.Config.ToString(), installation.GetExecutableFileName("DXC"));
                if (!File.Exists(dxcPath))
                {
                    throw TerminateException.Abort();
                }

                // Run DXC
                var options = new Terminal.Options
                {
                    Executable = dxcPath,
                    WorkingDirectory = resolver.Group.RootDirectory,
                    Logging = Terminal.Logging.All
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

        private async Task<List<SourceCodeDescriptor>> GenerateMakefileAsync(string makefilePath, string intDir, string outDir, CancellationToken cancellationToken)
        {
            var needsCompilation = new List<SourceCodeDescriptor>();
            var lines = new List<string>();
            
            lines.Add("# Auto-generated shader compilation makefile");
            lines.Add("# Format: <source> -t <type> -e <entrypoint> [--vulkan] -o <output> [-I <include_path>]...");
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
                var depsFile = Path.Combine(intDir, fileName + ".deps");

                // Check if compilation is needed
                if (await NeedsCompilationAsync(shaderFile.FilePath, outputBasePath, depsFile, cancellationToken))
                {
                    needsCompilation.Add(shaderFile);

                    // Add DirectX compilation
                    var includePaths = GetIncludePaths(shaderFile);
                    var includeArgs = string.Join(" ", includePaths.Select(p => $"-I \"{p}\""));
                    lines.Add($"\"{shaderFile.FilePath}\" -t {shaderType} -e main -o \"{outputBasePath}\" {includeArgs}");

                    // Add Vulkan compilation (SPIR-V)
                    lines.Add($"\"{shaderFile.FilePath}\" -t {shaderType} -e main --vulkan -o \"{outputBasePath}\" {includeArgs}");
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

        private async Task<bool> NeedsCompilationAsync(string sourceFile, string outputBasePath, string depsFile, CancellationToken cancellationToken)
        {
            var csoFile = outputBasePath + ".cso";
            var spvFile = outputBasePath + ".spv";

            // If output files don't exist, need compilation
            if (!File.Exists(csoFile) || !File.Exists(spvFile))
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
                var outputTime = File.GetLastWriteTimeUtc(csoFile);
                
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
            var resolver = m_Project.GetResolver(m_TargetInfo);
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
