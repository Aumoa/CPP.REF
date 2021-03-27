// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System.IO;
using System.Text;

/// <summary>
/// Xml 형식으로 된 프로젝트를 생성합니다.
/// </summary>
class XmlProjectBuilder
{
    StringBuilder Builder = new();

    protected XmlProjectBuilder()
    {
        Builder.AppendLine("<!-- Copyright 2020-2021 Aumoa.lib. All right reserved. -->");
        Builder.AppendLine("");
        Builder.AppendLine("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
    }

    public void Save(string InDest)
    {
        File.WriteAllText(InDest, Builder.ToString());
    }
}