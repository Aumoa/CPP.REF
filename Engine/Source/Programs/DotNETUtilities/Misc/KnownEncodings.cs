// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Text;

namespace AE.Misc;

public static class KnownEncodings
{
    static KnownEncodings()
    {
        Encoding.RegisterProvider(CodePagesEncodingProvider.Instance);
    }

    public static Encoding EUCKR => Encoding.GetEncoding(51949);
}
