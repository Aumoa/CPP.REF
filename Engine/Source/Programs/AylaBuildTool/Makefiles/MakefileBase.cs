// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Source;

namespace AE.Makefiles;

public abstract class MakefileBase
{
    public readonly ProjectDirectory Directory;
    public readonly string MakefilePath;

    public abstract string MakefileCachePath { get; }

    public MakefileBase(ProjectDirectory Directory, string MakefilePath)
    {
        this.Directory = Directory;
        this.MakefilePath = MakefilePath;
    }

    public virtual bool IsLoaded
    {
        get;
        protected set;
    }

    public abstract Task<bool> TryLoadAsync(object Arg, CancellationToken SToken = default);
}
