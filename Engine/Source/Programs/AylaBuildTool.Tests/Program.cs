namespace AylaEngine.Tests;

internal static class Program
{
    private static int Main()
    {
        List<string> failures = [];

        Run("dependencies on target line", failures, () =>
        {
            var actual = UnixInstallation.ParseMakeRuleDependencies(
                "obj/main.o: src/main.cpp include/CoreMinimal.h\r\n");
            AssertSequence(actual, "src/main.cpp", "include/CoreMinimal.h");
        });

        Run("dependencies on continuation lines", failures, () =>
        {
            var actual = UnixInstallation.ParseMakeRuleDependencies(
                "obj/main.o: src/main.cpp \\\r\n include/CoreMinimal.h \\\r\n include/PCH.h\r\n");
            AssertSequence(actual, "src/main.cpp", "include/CoreMinimal.h", "include/PCH.h");
        });

        Run("escaped paths and empty phony rules", failures, () =>
        {
            var actual = UnixInstallation.ParseMakeRuleDependencies(
                "C\\:/obj/main.o: src/main.cpp include/path\\ with\\ space.h C\\:/include/PCH.h\r\n" +
                "include/path\\ with\\ space.h:\r\n");
            AssertSequence(actual, "src/main.cpp", "include/path with space.h", "C:/include/PCH.h");
        });

        if (failures.Count == 0)
        {
            Console.WriteLine("AylaBuildTool.Tests passed.");
            return 0;
        }

        foreach (var failure in failures)
        {
            Console.Error.WriteLine(failure);
        }

        return 1;
    }

    private static void Run(string name, List<string> failures, Action test)
    {
        try
        {
            test();
            Console.WriteLine($"[PASS] {name}");
        }
        catch (Exception exception)
        {
            failures.Add($"[FAIL] {name}: {exception.Message}");
        }
    }

    private static void AssertSequence(string[] actual, params string[] expected)
    {
        if (actual.SequenceEqual(expected))
        {
            return;
        }

        throw new InvalidOperationException(
            $"Expected [{string.Join(", ", expected)}], actual [{string.Join(", ", actual)}].");
    }
}
