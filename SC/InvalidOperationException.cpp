using namespace SC;

InvalidOperationException::InvalidOperationException( String message, Exception* innerException, String name )
	: Exception( message, innerException, name )
{

}

InvalidOperationException::InvalidOperationException( Exception* innerException )
	: InvalidOperationException( "The requested operation cannot be performed.", innerException, "SC.InvalidOperationException" )
{

}

InvalidOperationException::InvalidOperationException( String message, Exception* innerException )
	: InvalidOperationException( message, innerException, "SC.InvalidOperationException" )
{

}