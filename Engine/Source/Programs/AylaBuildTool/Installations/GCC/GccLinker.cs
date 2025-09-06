
using System.Text;

namespace AylaEngine;

internal class GccLinker : Linker
{
    private readonly Installation m_Installation;
    private readonly TargetInfo m_TargetInfo;

    public GccLinker(Installation installation, TargetInfo targetInfo)
    {
        m_Installation = installation;
        m_TargetInfo = targetInfo;
    }

    public override async ValueTask<Terminal.Output> LinkAsync(ModuleRulesResolver module, CppCompiler.CompileItem[] sourceObjects, CancellationToken cancellationToken)
    {
        var options = new Terminal.Options
        {
            Executable = "g++",
            Logging = Terminal.Logging.None
        };

        var linkCommands = new StringBuilder();

        var outputPath = module.Group.Output(m_TargetInfo, FolderPolicy.PathType.Current);
        var outputFileName = module.Group.OutputFileName(m_Installation, m_TargetInfo, module.Rules.Name, module.Rules.Type, module.Rules.Scriptable.Enabled, FolderPolicy.PathType.Current);
        Directory.CreateDirectory(outputPath);

        if (module.Rules.IsSharedLibrary())
        {
            linkCommands.Append(
                "-shared "
            );
        }

        for (int i = 0; i < sourceObjects.Length; ++i)
        {
            var intermediateDirectory = sourceObjects[i].Descriptor.Intermediate(module.Name, m_TargetInfo, FolderPolicy.PathType.Current);
            var fileName = Path.GetFileName(sourceObjects[i].SourceCode.FilePath);
            var objectFileName = Path.Combine(intermediateDirectory, fileName + ".o");
            linkCommands.AppendFormat("\"{0}\" ", objectFileName);
        }
        
        linkCommands.AppendFormat("-o\"{0}\" ", outputFileName);

        string[] libPaths = [module.EngineGroup.Output(m_TargetInfo, FolderPolicy.PathType.Current), module.PrimaryGroup.Output(m_TargetInfo, FolderPolicy.PathType.Current)];
        foreach (var libPath in libPaths.Distinct())
        {
            linkCommands.AppendFormat("-L\"{0}\" ", libPath);
            linkCommands.AppendFormat("-Wl,-rpath,\"{0}\" ", libPath);
        }

        foreach (var additionalLibrary in module.AdditionalLibraries
            .Concat(module.DependencyModuleNames)
            .Append("stdc++")
            .Append("stdc++exp"))
        {
            linkCommands.AppendFormat("-l\"{0}\" ", additionalLibrary);
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