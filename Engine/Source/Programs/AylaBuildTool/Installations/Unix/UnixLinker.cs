using System.Text;

namespace AylaEngine;

internal abstract class UnixLinker : Linker
{
    protected readonly Installation m_Installation;
    protected readonly TargetInfo m_TargetInfo;

    protected UnixLinker(Installation installation, TargetInfo targetInfo)
    {
        m_Installation = installation;
        m_TargetInfo = targetInfo;
    }

    protected virtual ValueTask<string[]> ConfigureCommandsAsync(bool isShared, CancellationToken cancellationToken)
    {
        return ValueTask.FromResult<string[]>([]);
    }

    protected virtual ValueTask<string[]> ConfigureLibrariesAsync(IEnumerable<string> libraries, CancellationToken cancellationToken)
    {
        return ValueTask.FromResult<string[]>([]);
    }

    public override async ValueTask<Terminal.Output> LinkAsync(ModuleRulesResolver module, CppCompileCommand[] sourceObjects, CancellationToken cancellationToken)
    {
        var options = new Terminal.Options
        {
            Executable = await m_Installation.GetCompilerPath(m_TargetInfo, cancellationToken),
            Logging = Terminal.Logging.None
        };

        var linkCommands = new StringBuilder();

        var outputPath = module.Group.Output(m_TargetInfo, FolderPolicy.PathType.Current);
        var outputFileName = module.Group.OutputFileName(m_Installation, m_TargetInfo, module.Rules.Name, module.Rules.Type, FolderPolicy.PathType.Current);
        Directory.CreateDirectory(outputPath);

        foreach (var command in await ConfigureCommandsAsync(module.Rules.IsSharedLibrary(), cancellationToken))
        {
            linkCommands.Append(command + " ");
        }

        for (int i = 0; i < sourceObjects.Length; ++i)
        {
            linkCommands.AppendFormat("\"{0}\" ", sourceObjects[i].ObjectFilePath);
        }

        linkCommands.AppendFormat("-o\"{0}\" ", outputFileName);

        string[] libPaths = [module.EngineGroup.Output(m_TargetInfo, FolderPolicy.PathType.Current), module.PrimaryGroup.Output(m_TargetInfo, FolderPolicy.PathType.Current)];
        foreach (var libPath in libPaths.Distinct())
        {
            linkCommands.AppendFormat("-L\"{0}\" ", libPath);
            linkCommands.AppendFormat("-Wl,-rpath,\"{0}\" ", libPath);
        }

        foreach (var command in await ConfigureLibrariesAsync(module.AdditionalLibraries.Concat(module.DependencyModuleNames), cancellationToken))
        {
            linkCommands.Append(command + " ");
        }

        var result = await Terminal.ExecuteCommandAsync(linkCommands.ToString(), options, cancellationToken);
        if (result.IsCompletedSuccessfully && ((result.StdOut.Length == 0 && result.Logs.Length == 0) || (result.StdOut.Length == 1 && result.Logs.Length == 1 && string.IsNullOrWhiteSpace(result.StdOut[0].Value))))
        {
            Terminal.Log[] outputs =
            [
                new Terminal.Log
                {
                    Value = outputFileName,
                    Verbosity = Terminal.Verbose.Info
                }
            ];

            result = result with
            {
                StdOut = outputs,
                Logs = outputs
            };
        }

        return result;
    }
}
