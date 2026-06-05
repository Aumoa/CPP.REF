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

    public override async ValueTask<Terminal.Output> CompileAsync(CppCompileCommand command, CancellationToken cancellationToken = default)
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

        switch (m_TargetInfo.Config)
        {
            case Configuration.Debug:
            case Configuration.DebugGame:
                AddCompilerCommands("-Og", "-ggdb", "-fno-omit-frame-pointer", "-fno-inline", "-D_GLIBCXX_DEBUG");
                break;
            case Configuration.Development:
            case Configuration.Shipping:
                AddCompilerCommands("-O3");
                break;
        }

        List<string> includes = [];
        foreach (var includeDirectory in command.Environment.IncludePaths)
        {
            includes.Add($"-I\"{includeDirectory}\"");
        }

        AddCompilerCommands(includes.ToArray());

        List<string> macros = [];
        foreach (var macro in command.Environment.AdditionalMacros)
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

        Directory.CreateDirectory(command.IntermediateDirectory);

        AddCompilerCommands("-c");

        AddCompilerCommands($"{command.SourceCode.FilePath}");

        AddCompilerCommands($"-o\"{command.ObjectFilePath}\"");
        AddCompilerCommands($"-MMD -MF\"{command.DependenciesFilePath}\"");

        Terminal.Output output;
        using (await GetAccess(cancellationToken))
        {
            output = await Terminal.ExecuteCommandAsync(compileCommands.ToString(), options, cancellationToken);
        }

        if (output.ExitCode == 0)
        {
            var cached = await SourceCodeCache.MakeCachedAsync(m_Installation, command.SourceCode.FilePath, command.Resolver.RuleFilePath, command.DependenciesFilePath, command.Resolver.DependRuleFilePaths, cancellationToken);
            cached.SaveCached(command.CacheFilePath);
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
