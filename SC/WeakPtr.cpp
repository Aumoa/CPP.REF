using namespace SC;

WeakPtr::WeakPtr( Object* ptr )
{
	if ( ptr )
	{
		if ( !ptr->counter )
		{
			ptr->counter = new Details::ReferenceCounter();
		}

		this->ptr = ptr;
		counter = ptr->counter;
	}

	AddRef();
}

WeakPtr::~WeakPtr()
{
	Release();
}

WeakPtr::WeakPtr( const WeakPtr& copy )
{
	Release();

	if ( copy.counter )
	{
		ptr = copy.ptr;
		counter = copy.counter;
	}

	AddRef();
}

String WeakPtr::ToString()
{
	if ( IsValid )
	{
		return ptr->ToString();
	}
	else
	{
		return "SC.WeakPtr(null)";
	}
}

bool WeakPtr::IsValid_get()
{
	return counter && ( counter->weakCount ) != 0;
}

WeakPtr& WeakPtr::operator=( const WeakPtr& copy )
{
	Release();

	if ( copy.counter )
	{
		ptr = copy.ptr;
		counter = copy.counter;
	}

	AddRef();
	return *this;
}

void WeakPtr::AddRef()
{
	if ( counter )
	{
		counter->AddWeakRef();
	}
}

void WeakPtr::Release()
{
	if ( counter )
	{
		counter->ReleaseWeak();
		if ( counter->weakCount == 0 )
		{
			delete counter;
		}
		counter = nullptr;
	}
}

object WeakPtr::Resolve()
{
	if ( IsValid )
	{
		return ptr;
	}
	else
	{
		return nullptr;
	}
}