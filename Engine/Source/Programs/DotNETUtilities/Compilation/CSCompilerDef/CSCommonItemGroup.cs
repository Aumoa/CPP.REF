using System.Xml;

namespace AylaEngine;

public abstract record CSCommonItemGroup : CSElement
{
    public static CSCommonItemGroup Parse(XmlElement item)
    {
        return new CSRemoveItem(item.GetAttribute("Remove"));
    }
}
