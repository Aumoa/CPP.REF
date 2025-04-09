using System.Text;

namespace AylaEngine;

internal class MSLinker : Linker
{
    private readonly TargetInfo m_TargetInfo;
    private readonly VisualStudioInstallation.Product m_Product;
    private readonly StringBuilder m_CommandBuilder = new();

    public MSLinker(TargetInfo targetInfo, VisualStudioInstallation.Product product)
    {
        m_TargetInfo = targetInfo;
        m_Product = product;
    }

    public override async ValueTask<Terminal.Output> LinkAsync(ModuleRulesResolver module, Compiler.CompileItem[] sourceObjects, CancellationToken cancellationToken)
    {
        var options = new Terminal.Options
        {
            Executable = Path.Combine(m_Product.Directory, "bin", "Hostx64", m_TargetInfo.Platform.Architecture == Architecture.X64 ? "x64" : "x86", "link.exe"),
            Logging = Terminal.Logging.None
        };

        m_CommandBuilder.Clear();

        var outputPath = module.Group.Output(m_TargetInfo, FolderPolicy.PathType.Current);
        var outputFileName = module.Group.OutputFileName(m_TargetInfo, module.Rules.Name, module.Rules.Type, FolderPolicy.PathType.Current);
        Directory.CreateDirectory(outputPath);

        switch (module.Rules.Type)
        {
            case ModuleType.Library:
            case ModuleType.Game:
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
                break;
            case ModuleType.Application:
                m_CommandBuilder.Append(
                    // Suppresses display of sign-on banner.
                    "/nologo " +
                    // Set output file name.
                    $"/OUT:{outputFileName} " +
                    // Create debug symbols.
                    "/DEBUG "
                );
                break;
            default:
                throw TerminateException.Internal();
        }

        for (int i = 0; i < sourceObjects.Length; ++i)
        {
            var intermediateDirectory = sourceObjects[i].Descriptor.Intermediate(module.Name, m_TargetInfo, FolderPolicy.PathType.Current);
            var fileName = Path.GetFileName(sourceObjects[i].SourceCode.FilePath);
            var objectFileName = Path.Combine(intermediateDirectory, fileName + ".o");
            m_CommandBuilder.AppendFormat("\"{0}\" ", objectFileName);
        }

        string libraryPath = Path.Combine(m_Product.Directory, "lib", m_TargetInfo.Platform.Architecture switch
        {
            Architecture.X64 => "x64",
            _ => throw TerminateException.Internal()
        });

        foreach (var libPath in VisualStudioInstallation.GatherWindowsKitSharedLibrary(m_TargetInfo.Platform.Architecture)
            .Append(libraryPath)
            .Append(module.EngineGroup.Output(m_TargetInfo, FolderPolicy.PathType.Current))
            .Append(module.PrimaryGroup.Output(m_TargetInfo, FolderPolicy.PathType.Current))
            .Distinct())
        {
            m_CommandBuilder.Append($"/LIBPATH:\"{libPath}\" ");
        }

        foreach (var additionalLibrary in module.AdditionalLibraries.Concat(module.DependencyModuleNames.Select(p => p + ".lib")))
        {
            m_CommandBuilder.AppendFormat("\"{0}\" ", additionalLibrary);
        }

        var result = await Terminal.ExecuteCommandAsync(m_CommandBuilder.ToString(), options, cancellationToken);
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
