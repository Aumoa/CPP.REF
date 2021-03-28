// Copyright 2020-2021 Aumoa.lib. All right reserved.

public class ZlibBuild : BuildRules
{
    public ZlibBuild()
    {
        AdditionalPreprocessorDefines.AddRange(new string[]
        {
            "_CRT_SECURE_NO_WARNINGS",
            "_CRT_NONSTDC_NO_WARNINGS"
        });
    }
}