namespace Ayla;

public partial class GameInstance
{
    public virtual Scene? GetEntryScene()
    {
        return GetEntryScene__Internal();
    }
}
