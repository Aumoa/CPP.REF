// Copyright 2020-2023 Aumoa.lib. All right reserved.

using System.Diagnostics;

namespace AE.AppHelper;

public class App
{
    public readonly ProcessStartInfo PSI;
    private readonly bool bConsoleOut;

    public App(ProcessStartInfo PSI, bool bConsoleOut)
    {
        this.PSI = PSI;
        this.bConsoleOut = bConsoleOut;
    }

    public string Stdout { get; private set; } = string.Empty;

    public string Stderr { get; private set; } = string.Empty;

    public int ExitCode { get; private set; }

    public async Task StartAsync(CancellationToken InCancellationToken = default)
    {
        Process? P = new Process() { StartInfo = PSI };
        if (P == null)
        {
            throw new InvalidOperationException("Internal error.");
        }

        PSI.RedirectStandardOutput = true;
        PSI.RedirectStandardError = true;

        TaskCompletionSource TCS = new();
        P.EnableRaisingEvents = true;
        P.Exited += (_, _) => TCS.SetResult();
        if (InCancellationToken != default)
        {
            InCancellationToken.Register(() => TCS.SetCanceled());
        }

        P.Start();
        await (P.HasExited ? Task.CompletedTask : TCS.Task);

        (Stdout, Stderr) = (P.StandardOutput.ReadToEnd(), P.StandardError.ReadToEnd());
        if (this.bConsoleOut)
        {
            Console.Write(Stdout);
            Console.Error.Write(Stderr);
        }
        ExitCode = P.ExitCode;
    }

    public static async Task<App> Run(ProcessStartInfo PSI, bool bConsoleOut = false, CancellationToken InCancellationToken = default)
    {
        var A = new App(PSI, bConsoleOut);
        await A.StartAsync(InCancellationToken);
        return A;
    }
}
