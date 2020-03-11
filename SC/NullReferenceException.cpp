using namespace SC;

NullReferenceException::NullReferenceException( String message, Exception* innerException, String name )
	: Exception( message, innerException, name )
{

}

NullReferenceException::NullReferenceException( Exception* innerException )
	: NullReferenceException( "Object reference not set to an instance of an object.", innerException, "SC.NullReferenceException" )
{

}

NullReferenceException::NullReferenceException( String message, Exception* innerException )
	: NullReferenceException( message, innerException, "SC.NullReferenceException" )
{

}