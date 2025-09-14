namespace AylaEngine;

internal record SEnumDefine(CapturedContext Context, string Name, string? Value) : Syntax(Context, null)
{
    public static SEnumDefine Accept(Context context)
    {
        var capture = context.Capture();
        var nextLine = context.ExportWhile(c => c != ',' && c != '}').ToString();
        var items = nextLine.Split('=', 2, StringSplitOptions.TrimEntries | StringSplitOptions.RemoveEmptyEntries);
        return new SEnumDefine(capture, items[0], items.Length > 1 ? items[1] : null);
    }
}
