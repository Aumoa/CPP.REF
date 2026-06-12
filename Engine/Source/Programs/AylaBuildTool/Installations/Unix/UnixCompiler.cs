using System.Text;

namespace AylaEngine;

internal abstract class UnixCompiler : CppCompiler
{
    private readonly Installation m_Installation;
    private readonly TargetInfo m_TargetInfo;

    protected UnixCompiler(Installation installation, TargetInfo targetInfo)
    {
        m_Installation = installation;
        m_TargetInfo = targetInfo;
    }

    protected virtual ValueTask<string[]> GetCompilerArgumentsAsync(CancellationToken cancellationToken)
    {
        return ValueTask.FromResult<string[]>([]);
    }

    public override async ValueTask<Terminal.Output> CompileAsync(CompileItem item, CancellationToken cancellationToken = default)
    {
        var options = new Terminal.Options
        {
            Executable = await m_Installation.GetCompilerPath(m_TargetInfo, cancellationToken),
            Logging = Terminal.Logging.None
        };

        var compileCommands = new StringBuilder();

        AddCompilerCommands("-std=c++23", "-g", "-fPIC", "-Wno-invalid-offsetof");

        if (m_TargetInfo.Platform.Architecture == Architecture.X64)
        {
            AddCompilerCommands("-msse");
        }

        foreach (var argument in await GetCompilerArgumentsAsync(cancellationToken))
        {
            AddCompilerCommands(argument);
        }

        var profile = m_TargetInfo.Config.GetTargetProfile();
        switch (profile.Optimization)
        {
            case OptimizationMode.Debug:
                AddCompilerCommands("-Og", "-ggdb", "-fno-omit-frame-pointer", "-fno-inline");
                break;
            case OptimizationMode.Release:
                AddCompilerCommands("-O3");
                break;
        }

        if (profile.UsesDebugAbi)
        {
            AddCompilerCommands("-D_GLIBCXX_DEBUG");
        }

        List<string> includes = [];
        foreach (var includeDirectory in item.Resolver.IncludePaths)
        {
            includes.Add($"-I\"{includeDirectory}\"");
        }

        AddCompilerCommands(includes.ToArray());

        List<string> macros = [];
        foreach (var macro in item.Resolver.AdditionalMacros)
        {
            if (macro.Value == null)
            {
                macros.Add($"-D{macro.VarName}");
            }
            else
            {
                macros.Add($"-D{macro.VarName}=\"{macro.Value}\"");
            }
        }

        AddCompilerCommands(macros.ToArray());

        var fileName = Path.GetFileName(item.SourceCode.FilePath);
        var intermediateDirectory = item.Descriptor.Intermediate(item.Resolver.Name, m_TargetInfo, FolderPolicy.PathType.Current);
        var objectFileName = Path.Combine(intermediateDirectory, fileName + ".o");
        var depsFileName = Path.Combine(intermediateDirectory, fileName + ".deps");
        var cacheFileName = Path.Combine(intermediateDirectory, fileName + ".cache");

        Directory.CreateDirectory(intermediateDirectory);

        AddCompilerCommands("-c");

        AddCompilerCommands($"{item.SourceCode.FilePath}");

        AddCompilerCommands($"-o\"{objectFileName}\"");
        AddCompilerCommands($"-MMD -MF\"{depsFileName}\"");

        Terminal.Output output;
        using (await GetAccess(cancellationToken))
        {
            output = await Terminal.ExecuteCommandAsync(compileCommands.ToString(), options, cancellationToken);
        }

        if (output.ExitCode == 0)
        {
            var cached = await SourceCodeCache.MakeCachedAsync(m_Installation, item.SourceCode.FilePath, item.Resolver.RuleFilePath, depsFileName, item.Resolver.DependRuleFilePaths, cancellationToken);
            cached.SaveCached(cacheFileName);
        }

        return output;

        void AddCompilerCommands(params ReadOnlySpan<string?> args)
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
    }
}
