// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System;

namespace Ayla;

static class ScriptingLaunch
{
    public static int GuardedMain(Type launchType, string[] args)
    {
        BindingActivator.Register();

        var launch = Activator.CreateInstance(launchType) as Launch;
        if (launch == null)
        {
            throw new InvalidOperationException($"Cannot create '{launchType.FullName}' instance.");
        }

        Debug.LogInfoFormat("LogLaunch", "Test logging");
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
