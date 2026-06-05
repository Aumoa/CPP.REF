using System.Text;

namespace AylaEngine;

internal class ClCompiler : CppCompiler
{
    private readonly Installation m_Installation;
    private readonly TargetInfo m_TargetInfo;
    private readonly VisualStudioInstallation.Product m_Product;
    private readonly StringBuilder m_CommandBuilder = new();

    public ClCompiler(Installation installation, TargetInfo targetInfo, VisualStudioInstallation.Product product)
    {
        m_Installation = installation;
        m_TargetInfo = targetInfo;
        m_Product = product;
    }

    public override async ValueTask<Terminal.Output> CompileAsync(CppCompileCommand command, CancellationToken cancellationToken)
    {
        var options = new Terminal.Options
        {
            Executable = Path.Combine(m_Product.Directory, "bin", "Hostx64", m_TargetInfo.Platform.Architecture == Architecture.X64 ? "x64" : "x86", "cl.exe"),
            Logging = Terminal.Logging.None
        };

        m_CommandBuilder.Clear();

        m_CommandBuilder.Append(
            // Suppresses display of sign-on banner.
            "/nologo " +
            // Compiles without linking.
            "/c " +
            // C++23 standard.
            "/std:c++23preview " +
            // Enable C++ exception handling (no SEH exceptions) and extern "C" defaults to nothrow.
            "/EHsc " +
            // wchar_t is a native type, not a typedef.
            "/Zc:wchar_t " +
            // Checks buffer security.
            "/GS " +
            // Remove unreferenced functions or data if they're COMDAT or have internal linkage only.
            "/Zc:inline " +
            // Set output warning level to 4.
            "/W4 " +
            // Enable more security features and warnings.
            "/sdl " +
            // Disable treat warnings as errors.
            "/WX- " +
            // Set standard-conformance mode: no permissive
            "/permissive- " +
            // Displays the full path of source code files passed to cl.exe in diagnostic text.
            "/FC " +
            // Diagnostics format: prints column information.
            "/diagnostics:column " +
            // Use standard preprocessor.
            "/Zc:preprocessor " +
            // Causes the compiler to display the full path of source code files passed to the compiler in diagnostics.
            "/FC " +
            // Generates complete debugging information.
            "/Zi "
        );

        if (command.IsModuleInterface)
        {
            m_CommandBuilder.Append(
                // Enables C++ modules.
                "/exportModule "
            );
        }

        switch (m_TargetInfo.Config)
        {
            case Configuration.Debug:
            case Configuration.DebugGame:
                m_CommandBuilder.Append(
                    // Generates intrinsic functions
                    "/Oi- " +
                    // Disable optimization.
                    "/Od " +
                    // Enable fast runtime checks.
                    "/RTC1 " +
                    // Multithreaded DLL
                    "/MDd "
                );
                break;
            case Configuration.Development:
            case Configuration.Shipping:
                m_CommandBuilder.Append(
                    // Generates intrinsic functions
                    "/Oi " +
                    // Enables function-level linking.
                    "/Gy " +
                    // Creates fast code.
                    "/O2 " +
                    // Multithreaded DLL
                    "/MD "
                );
                break;
        }

        List<string> includes = [];
        foreach (var includeDirectory in command.Environment.IncludePaths
            .Append(Path.Combine(m_Product.Directory, "include"))
            .Concat(VisualStudioInstallation.GatherWindowsKitInclude()))
        {
            includes.Add($"/I\"{includeDirectory}\"");
        }

        m_CommandBuilder.Append(string.Join(' ', includes) + ' ');

        List<string> macros = [];
        foreach (var macro in command.Environment.AdditionalMacros)
        {
            if (macro.Value == null)
            {
                macros.Add($"/D{macro.VarName}");
            }
            else
            {
                macros.Add($"/D{macro.VarName}={macro.Value}");
            }
        }

        m_CommandBuilder.Append(string.Join(' ', macros) + ' ');

        List<string> disableWarnings = [];
        foreach (var disableWarning in command.Environment.DisableWarnings)
        {
            disableWarnings.Add($"/wd{disableWarning}");
        }

        if (disableWarnings.Count > 0)
        {
            m_CommandBuilder.Append(string.Join(' ', disableWarnings) + ' ');
        }

        Directory.CreateDirectory(command.IntermediateDirectory);

        var pdbFilePath = command.UsesPch && command.PchSettings != null
            ? command.PchSettings.PdbFilePath
            : command.PdbFilePath;

        m_CommandBuilder.AppendFormat(
            "/Fo\"{0}\" " +
            "/Fd\"{1}\" " +
            "/sourceDependencies \"{2}\" ",
            command.ObjectFilePath,
            pdbFilePath,
            command.DependenciesFilePath
        );

        if (command.CreatesPch)
        {
            var pchSettings = command.PchSettings
                ?? throw new InvalidOperationException("PCH compile command does not have PCH settings.");

            m_CommandBuilder.AppendFormat(
                "/Yc\"{0}\" " +
                "/Fp\"{1}\" " +
                "/FS ",
                pchSettings.HeaderIncludeName,
                pchSettings.PchFilePath
            );
        }
        else if (command.UsesPch)
        {
            var pchSettings = command.PchSettings
                ?? throw new InvalidOperationException("PCH compile command does not have PCH settings.");

            m_CommandBuilder.AppendFormat(
                "/Yu\"{0}\" " +
                "/Fp\"{1}\" " +
                "/FI\"{0}\" " +
                "/FS ",
                pchSettings.HeaderIncludeName,
                pchSettings.PchFilePath
            );
        }

        if (command.IsModuleInterface)
        {
            m_CommandBuilder.AppendFormat(
                "/ifcOutput \"{0}\" ",
                command.IntermediateDirectory
            );

            m_CommandBuilder.AppendFormat(
                "/ifcSearchDir \"{0}\" ",
                command.IntermediateDirectory
            );
        }

        m_CommandBuilder.AppendFormat("\"{0}\"", command.SourceCode.FilePath);
        Terminal.Output output;
        using (await GetAccess(cancellationToken))
        {
            output = await Terminal.ExecuteCommandAsync(m_CommandBuilder.ToString(), options, cancellationToken);
        }
        
        if (output.ExitCode == 0)
        {
            var cached = await SourceCodeCache.MakeCachedAsync(m_Installation, command.SourceCode.FilePath, command.Resolver.RuleFilePath, command.DependenciesFilePath, command.Resolver.DependRuleFilePaths, cancellationToken);
            cached.SaveCached(command.CacheFilePath);

            output = output with
            {
                Logs = [.. output.Logs.Skip(1)],
                StdOut = [.. output.StdOut.Skip(1)]
            };
        }
        else
        {
            var commandLog = new Terminal.Log
            {
                Value = $"cl.exe {m_CommandBuilder}",
                Verbosity = Terminal.Verbose.Info
            };

            output = output with
            {
                Logs = [commandLog, .. output.Logs],
                StdOut = [.. output.StdOut],
                StdErr = [.. output.StdErr]
            };
        }

        return output;
    }
}
