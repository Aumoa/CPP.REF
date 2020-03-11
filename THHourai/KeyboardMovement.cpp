using namespace Touhou;

KeyboardMovement::KeyboardMovement() : Component()
{

}

object KeyboardMovement::Clone()
{
	var clone = new KeyboardMovement();
	return clone;
}

void KeyboardMovement::Update( Time& time, Input& input )
{
	auto forw = Vector3::Forward;
	auto right = Vector3::Right;

	Vector3 mov;
	if ( input.KeyState[KeyCode::RightArrow] )
	{
		mov += right * time.DeltaTimeInSeconds * speed;
	}
	if ( input.KeyState[KeyCode::LeftArrow] )
	{
		mov -= right * time.DeltaTimeInSeconds * speed;
	}
	if ( input.KeyState[KeyCode::UpArrow] )
	{
		mov += forw * time.DeltaTimeInSeconds * speed;
	}
	if ( input.KeyState[KeyCode::DownArrow] )
	{
		mov -= forw * time.DeltaTimeInSeconds * speed;
	}

	Transform->Position += mov;
}