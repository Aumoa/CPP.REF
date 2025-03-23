namespace AylaEngine;

public static class ConsoleEnvironment
{
    private static object s_SynchronizationContext = new();
    private static bool? s_IsDynamic;

    public static bool IsDynamic
    {
        get
        {
            if (s_IsDynamic.HasValue == false)
            {
                lock (s_SynchronizationContext)
                {
                    if (s_IsDynamic.HasValue == false)
                    {
                        try
                        {
                            var lt = Console.CursorVisible;
                            Console.CursorVisible = true;
                            Console.CursorVisible = lt;
                            s_IsDynamic = true;
                        }
                        catch (IOException)
                        {
                            s_IsDynamic = false;
                        }
                    }
                }
            }

            return s_IsDynamic.Value;
        }
    }
}
