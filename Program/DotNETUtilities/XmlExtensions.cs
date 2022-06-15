// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Text;
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

    /// <summary>
    /// XML 문서를 파일에 저장합니다. 변경 사항이 있을 경우에만 쓰기 작업이 진행됩니다.
    /// </summary>
    /// <param name="doc"> 저장할 문서 개체를 전달합니다. </param>
    /// <param name="saveTo"> 저장할 위치가 기록된 파일 레퍼런스를 전달합니다. </param>
    /// <returns> 쓰기 작업이 실행되었으면 <see langword="true"/>가, 그 이외의 경우 <see langword="false"/>가 반환됩니다. </returns>
    public static bool SaveIfChanged(this XmlDocument doc, FileReference saveTo)
    {
        StringWriter stringWriter = new();
        XmlTextWriter xmlTextWriter = new(stringWriter)
        {
            Formatting = Formatting.Indented
        };

        doc.WriteTo(xmlTextWriter);
        return saveTo.WriteAllTextIfChanged(stringWriter.ToString());
    }
}
