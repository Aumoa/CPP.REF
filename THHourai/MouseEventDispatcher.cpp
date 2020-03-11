using namespace Touhou;

using namespace SC::Drawing;

MouseEventDispatcher::MouseEventDispatcher() : Component()
{

}

object MouseEventDispatcher::Clone()
{
	var clone = new MouseEventDispatcher();
	return clone;
}

void MouseEventDispatcher::Start()
{
	movement = GetComponent<Movement>();
}

void MouseEventDispatcher::Update( Time& time, Input& input )
{
	if ( input.IsKeyDown( KeyCode::Mouse1 ) )
	{
		var go = MainCamera::GetInstance();
		var cam = go->GetComponent<Camera>();

		Point<double> screenSpace = 0;
		auto clientSize = App::Instance()->ClientSize;
		
		screenSpace.X = ( double )input.MousePosition.X / ( double )clientSize.X;
		screenSpace.Y = ( double )input.MousePosition.Y / ( double )clientSize.Y;
		screenSpace.X = screenSpace.X * 2.0 - 1.0;
		screenSpace.Y = -( screenSpace.Y * 2.0 - 1.0 );

		auto ray = cam->ScreenSpaceToRay( screenSpace );
		double t = -ray.Origin.Y / ray.Direction.Y;
		Vector3 p = ray.Origin + ray.Direction * t;

		movement->MoveTo = p;
	}
}