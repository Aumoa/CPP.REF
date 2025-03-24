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

        var outputPath = Path.Combine(module.Group.BinariesDirectory, m_TargetInfo.Platform.Name, m_TargetInfo.Config.ToString());
        var outputFileName = Path.Combine(outputPath, module.Rules.Name);
        Directory.CreateDirectory(outputPath);

        switch (module.Rules.Type)
        {
            case ModuleType.Library:
                outputFileName += ".dll";
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
                outputFileName += ".exe";
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
            var intermediateDirectory = sourceObjects[i].Descriptor.GetIntermediateFolder(module, m_TargetInfo);
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
            .Append(outputPath))
        {
            m_CommandBuilder.Append($"/LIBPATH:\"{libPath}\" ");
        }

        foreach (var additionalLibrary in module.AdditionalLibraries.Concat(module.DependencyModuleNames.Select(p => p + ".lib")))
        {
            m_CommandBuilder.AppendFormat("\"{0}\" ", additionalLibrary);
        }

        return await Terminal.ExecuteCommandAsync(m_CommandBuilder.ToString(), options, cancellationToken);
    }
}
