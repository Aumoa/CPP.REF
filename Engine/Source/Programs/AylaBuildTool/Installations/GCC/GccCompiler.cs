using System.Text;

namespace AylaEngine;

internal class GccCompiler : CppCompiler
{
    private readonly TargetInfo m_TargetInfo;

    public GccCompiler(TargetInfo targetInfo)
    {
        m_TargetInfo = targetInfo;
    }

    public override async ValueTask<Terminal.Output> CompileAsync(CompileItem item, CancellationToken cancellationToken = default)
    {
        var options = new Terminal.Options
        {
            Executable = "gcc",
            Logging = Terminal.Logging.None
        };

        var compileCommands = new StringBuilder();
        var makefileCommands = new StringBuilder();

        AddCommands("-std=c++20", "-g");

        switch (m_TargetInfo.Config)
        {
            case Configuration.Debug:
            case Configuration.DebugGame:
                AddCompilerCommands("-Og");
                break;
            case Configuration.Development:
            case Configuration.Shipping:
                AddCompilerCommands("-O3", "-ffunction-sections", "-fdata", "-Wl,--gc-sections");
                break;
        }

        List<string> includes = [];
        foreach (var includeDirectory in item.Resolver.IncludePaths)
        {
            includes.Add($"-I\"{includeDirectory}\"");
        }

        AddCommands(includes.ToArray());

        var additionalMacros = item.Resolver.AdditionalMacros
            .Append("PLATFORM_LINUX=1")
            .Append("_UNICODE")
            .Append("UNICODE");

        if (m_TargetInfo.Config != Configuration.Shipping)
        {
            additionalMacros = additionalMacros.Append("DO_CHECK=1");
        }

        List<string> macros = [];
        foreach (var macro in additionalMacros)
        {
            if (macro.Value == null)
            {
                macros.Add($"-D{macro.VarName}");
            }
            else
            {
                macros.Add($"-D{macro.VarName}={macro.Value}");
            }
        }

        AddCommands(macros.ToArray());

        var fileName = Path.GetFileName(item.SourceCode.FilePath);
        var intermediateDirectory = item.Descriptor.Intermediate(item.Resolver.Name, m_TargetInfo, FolderPolicy.PathType.Current);
        var objectFileName = Path.Combine(intermediateDirectory, fileName + ".o");
        var depsFileName = Path.Combine(intermediateDirectory, fileName + ".deps");
        var cacheFileName = Path.Combine(intermediateDirectory, fileName + ".cache");

        Directory.CreateDirectory(intermediateDirectory);

        AddCompilerCommands("-c");
        AddMakefileCommands("-M");

        AddCommands($"{item.SourceCode.FilePath}");

        AddCompilerCommands($"-o\"{objectFileName}\"");
        AddMakefileCommands($"> \"{depsFileName}\"");

        Terminal.Output output;
        using (await GetAccess(cancellationToken))
        {
            output = await Terminal.ExecuteCommandAsync(makefileCommands.ToString(), options, cancellationToken);
        }

        using (await GetAccess(cancellationToken))
        {
            output = await Terminal.ExecuteCommandAsync(compileCommands.ToString(), options, cancellationToken);
        }
        
        if (output.ExitCode == 0)
        {
            var cached = await SourceCodeCache.MakeCachedAsync(item.SourceCode.FilePath, item.Resolver.RuleFilePath, depsFileName, item.Resolver.DependRuleFilePaths, cancellationToken);
            cached.SaveCached(cacheFileName);
        }

        return output;

        void AddCommands(params ReadOnlySpan<string?> args)
        {
            Internal__AddCommands(args, true, true);
        }

        void AddCompilerCommands(params ReadOnlySpan<string?> args)
        {
            Internal__AddCommands(args, true, false);
        }

        void AddMakefileCommands(params ReadOnlySpan<string?> args)
        {
            Internal__AddCommands(args, false, true);
        }

        void Internal__AddCommands(ReadOnlySpan<string?> args, bool addToCompiler, bool addToMakefile)
        {
            if (addToCompiler)
            {
                if (args.Length > 0)
                {
                    if (compileCommands.Length > 0)
                    {
                        compileCommands.Append(' ');
                    }
                    compileCommands.Append(string.Join(' ', args));
                }
            }

            if (addToMakefile)
            {
                if (args.Length > 0)
                {
                    if (makefileCommands.Length > 0)
                    {
                        makefileCommands.Append(' ');
                    }
                    makefileCommands.Append(string.Join(' ', args));
                }
            }
        }
    }
}