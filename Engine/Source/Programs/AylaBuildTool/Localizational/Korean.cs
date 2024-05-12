// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

internal class KoreanLocale : ILocale
{
    private static Errors InternalErrors = new();
    public IErrors Errors => InternalErrors;
}
