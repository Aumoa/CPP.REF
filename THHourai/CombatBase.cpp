using namespace Touhou;

CombatBase::CombatBase() : Component()
{

}

object CombatBase::Clone()
{
	var clone = new CombatBase();
	clone->maxHealthPoint = maxHealthPoint;
	clone->maxMagicPoint = maxMagicPoint;
	clone->currentHealthPoint = currentHealthPoint;
	clone->currentMagicPoint = currentMagicPoint;
	return clone;
}

double CombatBase::MaxHealthPoint_get()
{
	return maxHealthPoint;
}

void CombatBase::MaxHealthPoint_set( double value )
{
	maxHealthPoint = value;
}

double CombatBase::MaxMagicPoint_get()
{
	return maxMagicPoint;
}

void CombatBase::MaxMagicPoint_set( double value )
{
	maxMagicPoint = value;
}

double CombatBase::CurrentHealthPoint_get()
{
	return currentHealthPoint;
}

void CombatBase::CurrentHealthPoint_set( double value )
{
	currentHealthPoint = value;
}

double CombatBase::CurrentMagicPoint_get()
{
	return currentMagicPoint;
}

void CombatBase::CurrentMagicPoint_set( double value )
{
	currentMagicPoint = value;
}