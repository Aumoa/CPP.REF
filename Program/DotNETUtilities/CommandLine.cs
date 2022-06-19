// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

namespace DotNETUtilities;

/// <summary>
/// 명령줄 전달 매개변수를 해석합니다.
/// </summary>
public class CommandLine
{
    private Dictionary<char, List<string>> _shorts = null!;
    private Dictionary<string, List<string>> _expands = null!;
    private List<string> _unscoped = null!;

    private CommandLine()
    {
    }

    /// <summary>
    /// --로 시작하는 확장 매개변수에서 지정한 키가 설정되었는지 검사합니다.
    /// </summary>
    /// <param name="key"> 키를 전달합니다. </param>
    /// <returns> 키를 포함할 경우 <see langword="true"/>가 반환됩니다. </returns>
    public bool ContainsKey(string key) => _expands.ContainsKey(key);

    /// <summary>
    /// -로 시작하는 매개변수에서 지정한 키가 설정되었는지 검사합니다.
    /// </summary>
    /// <param name="key"> 키를 전달합니다. </param>
    /// <returns> 키를 포함할 경우 <see langword="true"/>가 반환됩니다. </returns>
    public bool ContainsKey(char key) => _shorts.ContainsKey(key);

    /// <summary>
    /// -로 시작하는 매개변수에서 지정한 키에 해당하는 값을 가져옵니다.
    /// </summary>
    /// <param name="key"> 키를 전달합니다. </param>
    /// <param name="value"> 키에 해당하는 값이 반환됩니다. 값이 없는 매개변수이거나 값이 2개 이상인 경우 <see langword="null"/>이 반환됩니다. </param>
    /// <returns> 값이 채워질 경우 <see langword="true"/>가 반환됩니다. </returns>
    public bool TryGetValue(char key, [MaybeNullWhen(false)] out string value)
    {
        value = null;
        if (!_shorts.TryGetValue(key, out var list))
        {
            return false;
        }

        if (list.Count == 1)
        {
            value = list[0];
            return true;
        }

        return false;
    }

    /// <summary>
    /// 범위가 지정되지 않은 매개변수 값을 가져옵니다.
    /// </summary>
    /// <param name="value"> 값이 반환됩니다. 값이 없거나 2개 이상인 경우 <see langword="null"/>이 반환됩니다. </param>
    /// <returns> 값이 채워질 경우 <see langword="true"/>가 반환됩니다. </returns>
    public bool TryGetValue([MaybeNullWhen(false)] out string value)
    {
        value = null;
        if (_unscoped.Count == 0)
        {
            return false;
        }

        if (_unscoped.Count == 1)
        {
            value = _unscoped[0];
            return true;
        }

        return false;
    }

    /// <summary>
    /// 콘솔 명령줄 데이터를 이용해 새 <see cref="CommandLine"/> 개체를 생성합니다.
    /// </summary>
    /// <param name="args"> 콘솔 명령줄 데이터를 전달합니다. </param>
    /// <returns> 생성된 <see cref="CommandLine"/> 개체가 반환됩니다. </returns>
    public static CommandLine FromConsoleArgs(string[] args)
    {
        Dictionary<char, List<string>> shorts = new();
        Dictionary<string, List<string>> expands = new();
        List<string> unscoped = new();

        List<string>? current = null;

        foreach (var value in args)
        {
            if (value.StartsWith("--"))
            {
                string key = value[2..];
                if (!expands.TryGetValue(key, out var list))
                {
                    list = new();
                    expands.Add(key, list);
                }

                current = list;
            }
            else if (value.StartsWith("-"))
            {
                if (value.Length == 2)
                {
                    char key = value[1];
                    if (!shorts.TryGetValue(key, out var list))
                    {
                        list = new();
                        shorts.Add(key, list);
                    }

                    current = list;
                }
                else
                {
                    foreach (var key in value[1..])
                    {
                        if (!shorts.TryGetValue(key, out _))
                        {
                            var list = new List<string>();
                            shorts.Add(key, list);
                        }
                    }
                }
            }
            else
            {
                (current ?? unscoped).Add(value);
            }
        }

        return new CommandLine()
        {
            _shorts = shorts,
            _expands = expands,
            _unscoped = unscoped
        };
    }
}
