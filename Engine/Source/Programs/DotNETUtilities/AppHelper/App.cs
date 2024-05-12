// Copyright 2020-2024 Aumoa.lib. All right reserved.

using System.Diagnostics;
using System.Text;

namespace AylaEngine;

public class App
{
    public readonly ProcessStartInfo m_PSI;
    private readonly bool m_WriteToConsole;

    public App(ProcessStartInfo psi, bool writeToConsole)
    {
        m_PSI = psi;
        m_WriteToConsole = writeToConsole;
    }

    public string Stdout { get; private set; } = string.Empty;

    public string Stderr { get; private set; } = string.Empty;

    public string Outputs { get; private set; } = string.Empty;

    public int ExitCode { get; private set; }

    public async Task StartAsync(CancellationToken cancellationToken = default)
    {
        Process? p = new Process() { StartInfo = m_PSI };
        if (p == null)
        {
            throw new InvalidOperationException("Internal error.");
        }

        m_PSI.RedirectStandardOutput = true;
        m_PSI.RedirectStandardError = true;

        TaskCompletionSource tcs = new();
        p.EnableRaisingEvents = true;
        p.Exited += (_, _) => tcs.SetResult();
        if (cancellationToken != default)
        {
            cancellationToken.Register(() => tcs.SetCanceled());
        }

        StringBuilder stdout = new();
        StringBuilder stderr = new();
        StringBuilder outputs = new();
        p.OutputDataReceived += (sender, args) =>
        {
            stdout.AppendLine(args.Data);
            outputs.AppendLine(args.Data);
            if (m_WriteToConsole)
            {
                Console.Write(args.Data);
            }
        };
        p.ErrorDataReceived += (sender, args) =>
        {
            stderr.AppendLine(args.Data);
            outputs.AppendLine(args.Data);
            if (m_WriteToConsole)
            {
                Console.Error.Write(args.Data);
            }
        };

        p.Start();
        p.BeginOutputReadLine();
        p.BeginErrorReadLine();

        while (p.HasExited == false)
        {
            await Task.WhenAny(tcs.Task, Task.Delay(1000));
        }

        Stdout = stdout.ToString();
        Stderr = stderr.ToString();
        Outputs = outputs.ToString();
        ExitCode = p.ExitCode;
    }

    public static async Task<App> Run(ProcessStartInfo psi, bool writeToConsole = false, CancellationToken cancellationToken = default)
    {
        var app = new App(psi, writeToConsole);
        await app.StartAsync(cancellationToken);
        return app;
    }
}
