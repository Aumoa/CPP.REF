// Copyright 2020-2022 Aumoa.lib. All right reserved.

using DotNETUtilities;

namespace BuildTool;

/// <summary>
/// 솔루션 단위 디렉토리의 레퍼런스를 표현합니다.
/// </summary>
public class SolutionDirectoryReference : DirectoryReference
{
    /// <inheritdoc/>
    public SolutionDirectoryReference(string path)
        : base(path)
    {
        Source = Move(nameof(Source));
        Intermediate = Move(nameof(Intermediate));
        Build = Move(nameof(Build));
    }

    /// <summary>
    /// <see cref="DirectoryReference"/> 개체로부터 새 인스턴스를 초기화합니다.
    /// </summary>
    /// <param name="from"> 복사본 개체를 전달합니다. </param>
    public SolutionDirectoryReference(DirectoryReference from)
        : base(from.FullPath)
    {
        Source = Move(nameof(Source));
        Intermediate = Move(nameof(Intermediate));
        Build = Move(nameof(Build));
    }

    /// <summary>
    /// 소스 디렉토리를 가져옵니다.
    /// </summary>
    public DirectoryReference Source { get; }

    /// <summary>
    /// 임시 디렉토리를 가져옵니다.
    /// </summary>
    public DirectoryReference Intermediate { get; }

    /// <summary>
    /// 빌드 디렉토리를 가져옵니다.
    /// </summary>
    public DirectoryReference Build { get; }
}
