// Copyright 2020-2022 Aumoa.lib. All right reserved.

using DotNETUtilities;

namespace BuildTool;

/// <summary>
/// 필요한 Rule 클래스를 대상 어셈블리에서 찾지 못했습니다.
/// </summary>
public class RuleClassException : Exception
{
    /// <summary>
    /// 개체의 새 인스턴스를 초기화합니다.
    /// </summary>
    /// <param name="classCode"> 클래스 코드 원본을 전달합니다. </param>
    /// <param name="requiredType"> 필요한 형식을 전달합니다. </param>
    public RuleClassException(FileReference classCode, Type requiredType)
        : base(FormatMessageNotFound(classCode, requiredType))
    {
        ClassCode = classCode;
    }

    /// <summary>
    /// 개체의 새 인스턴스를 초기화합니다.
    /// </summary>
    /// <param name="classCode"> 클래스 코드 원본을 전달합니다. </param>
    /// <param name="matchedType"> 일치한 형식을 전달합니다. </param>
    /// <param name="ctorTypes"> 필요한 생성자 형식 목록을 전달합니다. </param>
    public RuleClassException(FileReference classCode, Type matchedType, Type[] ctorTypes)
        : base(FormatMessageCtorMismatch(classCode, matchedType, ctorTypes))
    {
        ClassCode = classCode;
    }

    /// <summary>
    /// 개체의 새 인스턴스를 초기화합니다.
    /// </summary>
    /// <param name="classCode"> 클래스 코드 원본을 전달합니다. </param>
    /// <param name="matchedType"> 일치한 형식을 전달합니다. </param>
    /// <param name="innerException"> 생성자 실행 중 발생한 예외 개체를 전달합니다. </param>
    public RuleClassException(FileReference classCode, Type matchedType, Exception innerException)
        : base(FormatMessageException(classCode, matchedType, innerException))
    {
        ClassCode = classCode;
    }

    /// <summary>
    /// 예외를 발생시킨 규칙 코드 파일을 가져옵니다.
    /// </summary>
    public FileReference ClassCode { get; }

    private static string FormatMessageNotFound(FileReference classCode, Type requiredType)
    {
        string ruleName = classCode.Name.Split(".").First() + "Rule";
        return string.Format("'{0}' 규칙 코드에서 필요한 형식 '{1}'을 상속하는 클래스를 {2} 이름에서 찾지 못했습니다.", classCode.FullPath, requiredType.FullName, ruleName);
    }

    private static string FormatMessageCtorMismatch(FileReference classCode, Type matchedType, Type[] ctorTypes)
    {
        string requiredCtors;
        if (ctorTypes.Any())
        {
            requiredCtors = $"생성자 ({string.Join(", ", ctorTypes.Select(p => p.FullName))})";
        }
        else
        {
            requiredCtors = "기본 생성자";
        }

        return string.Format("'{0}' 규칙 코드에서 형식 '{1}'을 찾았지만 {2}를 포함하지 않습니다.", classCode.FullPath, matchedType.FullName, requiredCtors);
    }

    private static string FormatMessageException(FileReference classCode, Type requiredType, Exception innerException)
    {
        return string.Format("'{0}' 규칙 코드에서 형식 '{1}'을 인스턴스화 하는 중 '{2}' 예외가 발생했습니다.\n상세 메시지: {3}\nStackTrace: {4}", classCode.FullPath, requiredType.FullName, innerException.GetType().FullName, innerException.Message, innerException.StackTrace);
    }
}
