// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace DotNETUtilities;

/// <summary>
/// 대상 클래스에 로그 카테고리를 지정합니다.
/// </summary>
[AttributeUsage(AttributeTargets.Class)]
public class LogCategoryAttribute : Attribute
{
    /// <summary>
    /// 개체를 초기화합니다.
    /// </summary>
    public LogCategoryAttribute(string categoryName)
    {
        CategoryName = categoryName;
    }

    /// <summary>
    /// 카테고리 이름을 가져옵니다.
    /// </summary>
    public string CategoryName { get; }
}
