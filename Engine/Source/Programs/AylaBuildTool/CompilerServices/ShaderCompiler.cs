// Copyright 2020-2023 Aumoa.lib. All right reserved.

using System.Diagnostics;
using System.Text;

using AE.AppHelper;
using AE.BuildSettings;
using AE.CompilerServices.Makefiles;
using AE.Exceptions;

namespace AE.CompilerServices;

public class ShaderCompiler : Compiler
{
    public ShaderCompiler()
    {
    }

    public override async Task<string> CompileAsync(MakefileCompile node, CancellationToken cancellationToken = default)
    {
        var psi = GenerateStartInfo(node);
        DateTime startTime = DateTime.Now;
        var app = await App.Run(psi, cancellationToken: cancellationToken);
        if (app.ExitCode != 0)
        {
            throw new TerminateException(KnownErrorCode.CompileError, app.Outputs);
        }

        // shader compiler is not provide cache yet.
        node.Cache = null;

        return $"{app.Outputs.Trim(' ', '\t', '\n', '\r')}";
    }

    private static ProcessStartInfo GenerateStartInfo(MakefileCompile node)
    {
        var shaderOutputDirectory = node.ModuleInfo.ProjectDir.Intermediate.Includes.GetChild(node.ModuleInfo.Name);
        var psi = new ProcessStartInfo();
        psi.Arguments = $"-Input \"{node.SourceFile}\" -Output \"{shaderOutputDirectory}\"";
        psi.WorkingDirectory = ToolChain.GetShaderCompilerDirectory(Target.Platform.Architecture);

        if (Target.Platform.Group == PlatformGroup.Windows)
        {
            var directory = Global.EngineDirectory.Binaries.Root.GetHierarchy("Win64", "Shipping");
            psi.FileName = directory.GetFile("ShaderCompileWorker.exe");
        }
        else
        {
            throw new PlatformNotSupportedException();
        }

        return psi;
    }
}
