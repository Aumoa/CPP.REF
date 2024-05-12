// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public static class Locale
{
    private static readonly ILocale s_CurrentLocale;

    static Locale()
    {
        s_CurrentLocale = new KoreanLocale();
    }

    public static IErrors Errors => s_CurrentLocale.Errors;
}
