using System.Text;
using Spectre.Console;

namespace AylaEngine;

internal class ClCompiler : Compiler
{
    private readonly TargetInfo m_TargetInfo;
    private readonly VisualStudioInstallation.Product m_Product;
    private readonly StringBuilder m_CommandBuilder = new();

    public ClCompiler(TargetInfo targetInfo, VisualStudioInstallation.Product product)
    {
        m_TargetInfo = targetInfo;
        m_Product = product;
    }

    public override async ValueTask<Terminal.Output> CompileAsync(CompileItem item, CancellationToken cancellationToken)
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
            // C++20 standard ISO/IEC 14882:2020.
            "/std:c++20 " +
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
            "/FC "
        );

        switch (m_TargetInfo.Config)
        {
            case Configuration.Debug:
            case Configuration.DebugGame:
                m_CommandBuilder.Append(
                    // Generates intrinsic functions
                    "/Oi- " +
                    // Disable optimization.
                    "/Od " +
                    // Generates complete debugging information.
                    "/Zi " +
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
                    // Generates complete debugging information.
                    "/Zi " +
                    // Creates fast code.
                    "/O2 " +
                    // Multithreaded DLL
                    "/MD "
                );
                break;
        }

        List<string> includes = [];
        foreach (var includeDirectory in item.Resolver.IncludePaths
            .Append(Path.Combine(m_Product.Directory, "include"))
            .Concat(VisualStudioInstallation.GatherWindowsKitInclude()))
        {
            includes.Add($"/I\"{includeDirectory}\"");
        }

        m_CommandBuilder.Append(string.Join(' ', includes) + ' ');

        List<string> macros = [];
        foreach (var macro in item.Resolver.AdditionalMacros
            .Append("PLATFORM_WINDOWS=1")
            .Append("_UNICODE")
            .Append("UNICODE")
            )
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
        foreach (var disableWarning in item.Resolver.DisableWarnings)
        {
            disableWarnings.Add($"/wd{disableWarning}");
        }

        if (disableWarnings.Count > 0)
        {
            m_CommandBuilder.Append(string.Join(' ', disableWarnings) + ' ');
        }

        var fileName = Path.GetFileName(item.SourceCode.FilePath);
        var intermediateDirectory = item.Descriptor.Intermediate(item.Resolver.Name, m_TargetInfo, FolderPolicy.PathType.Current);
        var objectFileName = Path.Combine(intermediateDirectory, fileName + ".o");
        var pdbFileName = Path.Combine(intermediateDirectory, fileName + ".pdb");
        var depsFileName = Path.Combine(intermediateDirectory, fileName + ".deps.json");
        var cacheFileName = Path.Combine(intermediateDirectory, fileName + ".cache");

        Directory.CreateDirectory(intermediateDirectory);

        m_CommandBuilder.AppendFormat(
            "/Fo\"{0}\" " +
            "/Fd\"{1}\" " +
            "/sourceDependencies \"{2}\" ",
            objectFileName,
            pdbFileName,
            depsFileName
        );

        m_CommandBuilder.AppendFormat("\"{0}\"", item.SourceCode.FilePath);
        var output = await Terminal.ExecuteCommandAsync(m_CommandBuilder.ToString(), options, cancellationToken);
        if (output.ExitCode == 0)
        {
            var cached = await SourceCodeCache.MakeCachedAsync(item.SourceCode.FilePath, item.Resolver.RuleFilePath, depsFileName, cancellationToken);
            cached.SaveCached(cacheFileName);
        }

        return output;
    }
}
