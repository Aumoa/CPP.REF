using namespace Touhou;

using namespace std;

Movement::Movement() : Component()
{

}

object Movement::Clone()
{
	var clone = new Movement();
	clone->speed = speed;
	clone->moveTo = moveTo;
	return clone;
}

void Movement::Start()
{
	moveTo = Transform->Position;
	animator = GetComponent<Animator>();
}

void Movement::FixedUpdate( Time& fixedTime )
{
	auto deltaTime = fixedTime.FixedDeltaTimeInSeconds;
	auto current = Transform->Position;
	auto delta = moveTo - current;

	if ( delta.Magnitude > 0.01 )
	{
		auto distance = delta.Magnitude;
		auto speedT = speed * deltaTime;

		delta = delta.Normalized;
		auto range = min( speedT, distance );
		auto move = delta * range;

		Transform->Position = current + move;
		Transform->LookTo( delta );

		if ( animator )
		{
			animator->Vars[L"Velocity"] = 1.0;
		}
	}
	else
	{
		animator->Vars[L"Velocity"] = 0.0;
	}
}

double Movement::Velocity_get()
{
	return speed;
}

void Movement::Velocity_set( double value )
{
	speed = value;
}

void Movement::MoveTo_set( Vector3 right )
{
	moveTo = right;
}