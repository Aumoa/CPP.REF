// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Text;

namespace DotNETUtilities;

/// <summary>
/// 콘솔 화면에 진행률을 표시하는 기능을 제공합니다.
/// </summary>
public class ConsoleProgressBar
{
    private double _progress = 0;
    private ConsoleCursorPin? _pin = null;

    ConsoleProgressBar()
    {
    }

    /// <summary>
    /// 진행률 개체의 표시 이름을 설정하거나 가져옵니다.
    /// </summary>
    public string Name { get; set; } = null!;

    /// <summary>
    /// 진행률 개체의 표시 텍스트를 설정하거나 가져옵니다.
    /// </summary>
    public string Text { get; set; } = null!;

    /// <summary>
    /// 현재 진행률 상태를 설정하거나 가져옵니다.
    /// </summary>
    public double Progress
    {
        get => _progress;
        set
        {
            value = Math.Clamp(value, 0, 1);
            if (_pin is null)
            {
                _progress = value;
            }
            else
            {
                string name = Name;
                string text = Text;

                while (true)
                {
                    double previousValue = _progress;
                    double delta = value - previousValue;

                    if (Interlocked.CompareExchange(ref _progress, value, previousValue) != previousValue)
                    {
                        continue;
                    }

                    if (Math.Abs(delta) >= TickRate || value == 0 || value == 1)
                    {
                        Log.Action(() => RenderBar(_pin, name, text, value));
                    }

                    break;
                }
            }
        }
    }

    /// <summary>
    /// 완료 단계를 지정하여 진행률 상태를 계산합니다.
    /// </summary>
    /// <param name="progress"> 현재 진행률 상태를 전달합니다. </param>
    /// <param name="goal"> 완료 상태를 전달합니다. </param>
    public void SetProgress(double progress, double goal)
    {
        Progress = progress / goal;
    }

    private static void RenderBar(ConsoleCursorPin pin, string name, string text, double progress)
    {
        int width = Console.BufferWidth - (name.Length + 1) - "[](100.00%)".Length;
        int filled = Math.Max((int)(width * progress), 1) - 1;

        StringBuilder sb = new();
        if (filled >= text.Length)
        {
            sb.Append("<color=Green>");
            sb.Append(new string('=', filled - text.Length));
            sb.Append("</color>");
            sb.Append(text);
            sb.Append("<color=Green>></color>");

            filled += 1;
        }
        else
        {
            sb.Append("<color=Green>");
            sb.Append(new string('=', filled));
            sb.Append('>');
            sb.Append("</color>");
            sb.Append(text);

            filled += text.Length + 1;
        }

        sb.Append(new string(' ', width - filled));
        ConsoleEx.WriteFormatted(pin, $"{name}[{sb}]({progress * 100.0:F2}%)");
    }

    /// <summary>
    /// 진행 상태의 업데이트를 위한 최소 변화량을 설정하거나 가져옵니다.
    /// </summary>
    public double TickRate { get; set; } = 0.01;

    /// <summary>
    /// 현재 콘솔 위치에 진행률 표시 공간을 예약하고 해당 공간을 사용하는 진행률 표시 개체를 생성합니다.
    /// </summary>
    /// <param name="name"> 진행률 표시 이름을 전달합니다. </param>
    /// <returns> 개체가 반환됩니다. </returns>
    public static ConsoleProgressBar Create(string name = "")
    {
        return new ConsoleProgressBar()
        {
            Name = name,
            Text = "",
            _pin = ConsoleCursorPin.Create()
        };
    }

    /// <summary>
    /// 화면에 표시되지 않는 진행률 표시 개체를 생성합니다.
    /// </summary>
    /// <returns> 개체가 반환됩니다. </returns>
    public static ConsoleProgressBar Null()
    {
        return new ConsoleProgressBar();
    }
}
