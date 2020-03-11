using namespace SC;

Exception::Exception()
	: name( "SC.Exception" )
	, message( "" )
{

}

Exception::Exception( String message, Exception* innerException )
	: name( "SC.Exception" )
	, message( message )
	, innerException( innerException )
{

}

Exception::Exception( String message, Exception* innerException, String name )
	: name( name )
	, message( message )
	, innerException( innerException )
{

}

String Exception::ToString()
{
	auto msg = String::Format( "{0}: {1}", Name, Message );
	if ( innerException )
	{
		msg = String::Format( "{0}\n--> {1}", msg, innerException->ToString() );
	}

	return msg;
}

String Exception::Name_get()
{
	return name;
}

String Exception::Message_get()
{
	return message;
}

RefPtr<Exception> Exception::InnerException_get()
{
	return innerException;
}