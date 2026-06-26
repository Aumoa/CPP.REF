using System.Text;

namespace AylaEngine;

internal class MSLinker : Linker
{
    private readonly Installation m_Installation;
    private readonly TargetInfo m_TargetInfo;
    private readonly VisualStudioInstallation.Product m_Product;
    private readonly StringBuilder m_CommandBuilder = new();

    public MSLinker(Installation installation, TargetInfo targetInfo, VisualStudioInstallation.Product product)
    {
        m_Installation = installation;
        m_TargetInfo = targetInfo;
        m_Product = product;
    }

    public override async ValueTask<Terminal.Output> LinkAsync(ModuleRulesResolver module, CppCompileCommand[] sourceObjects, CancellationToken cancellationToken)
    {
        var options = new Terminal.Options
        {
            Executable = Path.Combine(m_Product.Directory, "bin", "Hostx64", m_TargetInfo.Platform.Architecture == Architecture.X64 ? "x64" : "x86", "link.exe"),
            Logging = Terminal.Logging.None
        };

        m_CommandBuilder.Clear();

        var outputPath = module.Group.ModuleOutput(m_TargetInfo, module.BuildProfile, FolderPolicy.PathType.Current);
        var outputFileName = module.Group.ModuleOutputFileName(m_Installation, m_TargetInfo, module.BuildProfile, module.Rules.Name, module.Rules.Type, FolderPolicy.PathType.Current);
        Directory.CreateDirectory(outputPath);

        if (module.Rules.IsSharedLibrary())
        {
            m_CommandBuilder.Append(
                // Suppresses display of sign-on banner.
                "/nologo " +
                // Create a DLL.
                "/DLL " +
                // Set output file name.
                $"/OUT:{outputFileName} " +
                // Create debug symbols.
                "/DEBUG "
            );
        }
        else
        {
            m_CommandBuilder.Append(
                // Suppresses display of sign-on banner.
                "/nologo " +
                // Set output file name.
                $"/OUT:{outputFileName} " +
                // Create debug symbols.
                "/DEBUG "
            );
        }

        for (int i = 0; i < sourceObjects.Length; ++i)
        {
            m_CommandBuilder.AppendFormat("\"{0}\" ", sourceObjects[i].ObjectFilePath);
        }

        string libraryPath = Path.Combine(m_Product.Directory, "lib", m_TargetInfo.Platform.Architecture switch
        {
            Architecture.X64 => "x64",
            _ => throw TerminateException.Internal()
        });

        foreach (var libPath in VisualStudioInstallation.GatherWindowsKitSharedLibrary(m_TargetInfo.Platform.Architecture)
            .Append(libraryPath)
            .Append(module.EngineGroup.ModuleOutput(m_TargetInfo, BuildProfileResolver.Resolve(module.EngineGroup, m_TargetInfo), FolderPolicy.PathType.Current))
            .Append(module.PrimaryGroup.ModuleOutput(m_TargetInfo, BuildProfileResolver.Resolve(module.PrimaryGroup, m_TargetInfo), FolderPolicy.PathType.Current))
            .Distinct())
        {
            m_CommandBuilder.Append($"/LIBPATH:\"{libPath}\" ");
        }

        foreach (var additionalLibrary in module.AdditionalLibraries.Concat(module.DependencyModuleNames.Select(p => p + ".lib")))
        {
            m_CommandBuilder.AppendFormat("\"{0}\" ", additionalLibrary);
        }

        var commandParameters = m_CommandBuilder.ToString();
        var result = await Terminal.ExecuteCommandAsync(commandParameters, options, cancellationToken);
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

            result = result.Replace(outputs);
        }

        return result;
    }
}
