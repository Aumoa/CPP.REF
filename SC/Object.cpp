using namespace SC;

using namespace std;

atomic<int> Object::instanceCount = 0;

size_t Object::AddRef()
{
	counter->AddRef();
	return ( size_t )counter->refCount;
}

size_t Object::Release()
{
	counter->Release();

	if ( counter->refCount == 0 )
	{
		if ( locked == false )
		{
			if ( counter->weakCount == 0 )
			{
				delete counter;
			}
			counter = nullptr;
			delete this;
		}
		else
		{
			//locked = false;
		}
		return 0;
	}
	else
	{
		return ( size_t )counter->refCount;
	}
}

Object::Object()
{
	instanceCount += 1;
}

Object::~Object()
{
	instanceCount -= 1;

	if ( counter && counter->weakCount == 0 )
	{
		delete counter;
		counter = nullptr;
	}
}

String Object::ToString()
{
	return L"SC.Object";
}

bool Object::Equals( object obj )
{
	return ReferenceEquals( this, obj );
}

bool Object::IsLocked_get()
{
	return locked;
}

void Object::IsLocked_set( bool value )
{
	locked = value;
}

String Object::ToString( object obj )
{
	return obj->ToString();
}

bool Object::Equals( object objA, object objB )
{
	if ( objA.IsValid == false )
	{
		return !objB.IsValid;
	}
	else
	{
		return objA->Equals( objB );
	}
}

bool Object::ReferenceEquals( object objA, object objB )
{
	return objA.Get() == objB.Get();
}

int Object::GetInstanceCount()
{
	return instanceCount;
}

bool Object::operator==( object right )
{
	return Equals( right );
}

bool Object::operator!=( object right )
{
	return !Equals( right );
}