namespace Ayla;

public partial class GenericApplication
{
    private static GenericApplication? s_App;

    partial void OnConstructed()
    {
        s_App = this;
    }

    public static GenericApplication Get()
    {
        if (s_App == null)
        {
            throw new InvalidOperationException("GenericApplication is not constructed yet.");
        }

        return s_App;
    }
}
