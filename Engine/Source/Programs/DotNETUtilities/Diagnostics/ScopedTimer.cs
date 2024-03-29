﻿// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;

namespace AE.Diagnostics;

public static class ScopedTimer
{
    private static readonly Stopwatch s_Stopwatch = new();

    public static void Start()
    {
        s_Stopwatch.Restart();
    }

    public static TimeSpan Stop()
    {
        s_Stopwatch.Stop();
        return s_Stopwatch.Elapsed;
    }
}
