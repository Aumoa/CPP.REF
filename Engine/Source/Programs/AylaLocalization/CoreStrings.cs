// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Locales;

namespace AE;

public static class CoreStrings
{
    private static readonly ILocale CurrentLocale;

    static CoreStrings()
    {
        CurrentLocale = new KoreanLocale();
    }

    public static IErrors Errors => CurrentLocale.Errors;
}
