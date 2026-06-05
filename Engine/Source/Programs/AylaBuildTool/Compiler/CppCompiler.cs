using System.Diagnostics;

namespace AylaEngine;

internal abstract class CppCompiler
{
    protected readonly struct ScopedAccess : IDisposable
    {
        private readonly SemaphoreSlim? m_Instance;

        private ScopedAccess(SemaphoreSlim s)
        {
            m_Instance = s;
        }

        public void Dispose()
        {
            Debug.Assert(m_Instance != null);
            m_Instance.Release();
        }

        public static async ValueTask<ScopedAccess> GetAccess(SemaphoreSlim s, CancellationToken cancellationToken = default)
        {
            await s.WaitAsync(cancellationToken);
            return new ScopedAccess(s);
        }
    }

    private static SemaphoreSlim s_Access;

    static CppCompiler()
    {
        int hardwareConcurrency = Environment.ProcessorCount;
        s_Access = new SemaphoreSlim(hardwareConcurrency);
    }

    public abstract ValueTask<Terminal.Output> CompileAsync(CppCompileCommand command, CancellationToken cancellationToken = default);

    protected static ValueTask<ScopedAccess> GetAccess(CancellationToken cancellationToken = default)
    {
        return ScopedAccess.GetAccess(s_Access, cancellationToken);
    }
}
