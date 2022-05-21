// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Xml;

namespace DotNETUtilities;

/// <summary>
/// <see cref="XmlDocument"/> 및 <see cref="XmlNode"/> 개체에 대한 공통 확장 함수를 제공합니다.
/// </summary>
public static class XmlExtensions
{
    /// <summary>
    /// 새 XML 선언 노드를 생성하고 문서에 바로 추가합니다. 선언 노드는 언제나 최상단에 삽입됩니다.
    /// </summary>
    /// <returns> 생성된 선언 노드 개체가 반환됩니다. </returns>
    public static XmlDeclaration NewDeclaration(this XmlDocument doc)
    {
        XmlDeclaration decl = doc.CreateXmlDeclaration("1.0", "utf-8", null);
        if (doc.FirstChild != null)
        {
            doc.InsertBefore(doc.FirstChild, decl);
        }
        else
        {
            doc.AppendChild(decl);
        }
        return decl;
    }
}
