// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Locales.Korean;

namespace AE.Locales;

internal class KoreanLocale : ILocale
{
    private static Errors InternalErrors = new();
    public IErrors Errors => InternalErrors;
}
