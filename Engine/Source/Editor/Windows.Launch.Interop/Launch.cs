// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public static class Launch
{
    private static void Main(string[] args)
    {
        using var applicationScope = GenericApplication.CreateApplication();
        using var windowScope = applicationScope.MakeWindow();
        windowScope.Show();
    }
}
