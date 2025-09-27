using System.Reflection;
using System.Xml;

namespace AylaEngine;

internal static partial class BuildRunner
{
    private class ScriptTask(ModuleRulesResolver resolver) : ITask
    {
        private readonly TaskCompletionSource m_CompletionSource = new();

        public GroupDescriptor Group => resolver.Group;

        public ModuleRulesResolver Resolver => resolver;

        public Task Task => m_CompletionSource.Task;

        public bool NeedBuild(TargetInfo targetInfo)
        {
            var assemblyName = Resolver.Name + ".Script";
            var sourceDirectory = resolver.Project.ScriptSourceDirectory;
            string intDir = Group.Intermediate(assemblyName, targetInfo, FolderPolicy.PathType.Current);

            if (Directory.Exists(intDir) == false)
            {
                return true;
            }

            foreach (var sourceFile in GatherSourceCodes(sourceDirectory))
            {
                var relativeFileName = Path.GetRelativePath(sourceDirectory, sourceFile);
                var fileId = relativeFileName.Replace(Path.DirectorySeparatorChar, '_');
                var cacheFileName = Path.Combine(intDir, fileId + ".cache");
                if (File.Exists(cacheFileName) == false)
                {
                    return true;
                }

                var previous = SourceCodeCache.LoadCached(cacheFileName);
                var @new = SourceCodeCache.MakeCachedSimple(sourceFile, Resolver.RuleFilePath);
                if (previous.IsModified(@new))
                {
                    return true;
                }
            }

            return false;
        }

        public async Task<Terminal.Output> BuildAsync(IList<ScriptTask> scriptTasks, TargetInfo targetInfo, CancellationToken cancellationToken)
        {
            try
            {
                try
                {
                    foreach (var name in resolver.DependencyModuleNames)
                    {
                        var task = scriptTasks.Where(p => p.Resolver.Name == name).FirstOrDefault();
                        if (task != null)
                        {
                            await task.Task;
                        }
                    }
                }
                catch (CSCompilerError)
                {
                    // ignore compilation error of dependency module
                    m_CompletionSource.SetCanceled();
                    throw new OperationCanceledException();
                }

                var csprojXml = new XmlDocument();
                csprojXml.Load(resolver.Project.ScriptProjectFileName);

                var config = VSUtility.GetConfigName(targetInfo);
                var platform = targetInfo.Platform.Name;
                var assemblyName = resolver.Name + ".Script";
                var condition = CSCondition.Parse($"$(Configuration)|$(Platform)=='{config}|{platform}'");

                var csproj = CSProject.Parse(csprojXml.OfType<XmlElement>().First()).Freeze(condition);
                var sourceCodes = GatherSourceCodes(resolver.Project.ScriptSourceDirectory)
                    .Select(CSSourceCode.FromFile)
                    .Append(csproj.GenerateAssemblyAttribute(null, config, null, assemblyName, Version.Parse("1.0.0.0")))
                    .Append(csproj.GenerateGlobals());

                var outputFileName = await CSCompiler.CompileAsAsync(sourceCodes, csproj, resolver.Project.ScriptSourceDirectory, resolver.Name + ".Script", cancellationToken);
                Terminal.Log[] logs = [new() { Verbosity = Terminal.Verbose.Info, Value = outputFileName }];
                GenerateCache(targetInfo);
                m_CompletionSource.SetResult();
                return new Terminal.Output
                {
                    Executable = "",
                    Command = "",
                    ExitCode = 0,
                    Logs = logs,
                    StdOut = logs,
                    StdErr = []
                };
            }
            catch (OperationCanceledException)
            {
                m_CompletionSource.SetCanceled();
                throw;
            }
            catch (CSCompilerError e)
            {
                Console.Error.WriteLine(e.Message);
                m_CompletionSource.SetException(e);
                throw;
            }
            catch (Exception e)
            {
                m_CompletionSource.SetException(e);
                throw;
            }
        }

        private void GenerateCache(TargetInfo targetInfo)
        {
            var assemblyName = Resolver.Name + ".Script";
            var sourceDirectory = resolver.Project.ScriptSourceDirectory;
            string intDir = Group.Intermediate(assemblyName, targetInfo, FolderPolicy.PathType.Current);

            if (Directory.Exists(intDir))
            {
                Directory.Delete(intDir, true);
            }

            Directory.CreateDirectory(intDir);

            foreach (var sourceFile in GatherSourceCodes(sourceDirectory))
            {
                var relativeFileName = Path.GetRelativePath(sourceDirectory, sourceFile);
                var fileId = relativeFileName.Replace(Path.DirectorySeparatorChar, '_');
                var cacheFileName = Path.Combine(intDir, fileId + ".cache");
                var current = SourceCodeCache.MakeCachedSimple(sourceFile, resolver.RuleFilePath);
                current.SaveCached(cacheFileName);
            }
        }

        private static IEnumerable<string> GatherSourceCodes(string sourceDirectory)
        {
            var allSourceFiles = Directory.GetFiles(sourceDirectory, "*.*", SearchOption.AllDirectories);
            var objDir = Path.GetFullPath(Path.Combine(sourceDirectory, "obj"));
            var binDir = Path.GetFullPath(Path.Combine(sourceDirectory, "bin"));

            foreach (var sourceFile in allSourceFiles)
            {
                if (sourceFile.StartsWith(objDir))
                {
                    continue;
                }

                if (sourceFile.StartsWith(binDir))
                {
                    continue;
                }

                var ext = Path.GetExtension(sourceFile);
                if (ext.Equals(".cs", StringComparison.OrdinalIgnoreCase))
                {
                    yield return sourceFile;
                }
            }
        }
    }
}
