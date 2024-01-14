// Copyright 2020-2023 Aumoa.lib. All right reserved.

using System.Diagnostics;
using System.Text;

namespace AE.AppHelper;

public class App
{
    public readonly ProcessStartInfo psi;
    private readonly bool writeToConsole;

    public App(ProcessStartInfo psi, bool writeToConsole)
    {
        this.psi = psi;
        this.writeToConsole = writeToConsole;
    }

    public string Stdout { get; private set; } = string.Empty;

    public string Stderr { get; private set; } = string.Empty;

    public int ExitCode { get; private set; }

    public async Task StartAsync(CancellationToken cancellationToken = default)
    {
        Process? p = new Process() { StartInfo = psi };
        if (p == null)
        {
            throw new InvalidOperationException("Internal error.");
        }

        psi.RedirectStandardOutput = true;
        psi.RedirectStandardError = true;

        TaskCompletionSource tcs = new();
        p.EnableRaisingEvents = true;
        p.Exited += (_, _) => tcs.SetResult();
        if (cancellationToken != default)
        {
            cancellationToken.Register(() => tcs.SetCanceled());
        }

        StringBuilder stdout = new();
        StringBuilder stderr = new();
        p.OutputDataReceived += (sender, args) => stdout.AppendLine(args.Data);
        p.ErrorDataReceived += (sender, args) => stderr.AppendLine(args.Data);

        p.Start();
        p.BeginOutputReadLine();
        p.BeginErrorReadLine();

        while (p.HasExited == false)
        {
            await Task.WhenAny(tcs.Task, Task.Delay(1000));
        }

        Stdout = stdout.ToString();
        Stderr = stderr.ToString();

        if (writeToConsole)
        {
            Console.Write(Stdout);
            Console.Error.Write(Stderr);
        }
        ExitCode = p.ExitCode;
    }

    public static async Task<App> Run(ProcessStartInfo psi, bool writeToConsole = false, CancellationToken cancellationToken = default)
    {
        var app = new App(psi, writeToConsole);
        await app.StartAsync(cancellationToken);
        return app;
    }
}
