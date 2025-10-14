namespace Ayla;

public partial class GameObject
{
    public T AddComponent<T>() where T : Component, IStaticObject
    {
        return (T)AddComponent(T.GetManagedType());
    }
}
