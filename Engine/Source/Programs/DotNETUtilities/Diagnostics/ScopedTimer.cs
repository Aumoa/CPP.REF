// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;

namespace AE.Diagnostics;

public class ScopedTimer : IDisposable
{
    private static readonly ThreadLocal<Dictionary<string, double>> Results = new(() => new());

    private readonly string Name;
    private readonly Stopwatch? Timer;

    public ScopedTimer(string InName, bool bStartNew = true)
    {
        Name = InName;
        if (bStartNew)
        {
            Timer = Stopwatch.StartNew();
        }
    }

    public void Dispose()
    {
        Debug.Assert(Timer != null);
        Timer.Stop();
        Results.Value!.TryAdd(Name, Timer.Elapsed.TotalSeconds);
        GC.SuppressFinalize(this);
    }

    public static double GetElapsed(string InName)
    {
        Results.Value!.TryGetValue(InName, out double Val);
        return Val;
    }
}
