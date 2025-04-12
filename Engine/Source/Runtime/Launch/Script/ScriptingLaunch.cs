// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace Ayla;

static class ScriptingLaunch
{
    public static int GuardedMain(Launch launch, string[] args)
    {
        try
        {
            return launch.StartApplication();
        }
        finally
        {
            launch.Dispose();
            GC.Collect();
            GC.WaitForCompleteToFinalize();
        }
    }
}
