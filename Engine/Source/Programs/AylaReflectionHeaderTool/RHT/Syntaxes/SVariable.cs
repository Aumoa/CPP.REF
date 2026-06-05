using System.Text.RegularExpressions;

namespace AylaEngine.RHT;

internal record SVariable(CapturedContext Context, STypeName TypeName, string Name, string? DefaultValue) : Syntax(Context, null)
{
    public static SVariable Accept(Context context, Predicate<char> escape)
    {
        var capture = context.Capture();
        STypeName typeName = STypeName.Accept(context);
        var nextLine = context.ExportWhile(c => !escape(c)).ToString();
        var items = nextLine.Split('=', 2, StringSplitOptions.TrimEntries | StringSplitOptions.RemoveEmptyEntries);
        return new SVariable(capture, typeName, items[0], items.Length > 1 ? items[1] : null);
    }
}
