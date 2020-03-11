using namespace SC;

InvalidCastException::InvalidCastException( String message, Exception* innerException, String name )
	: Exception( message, innerException, name )
{

}

InvalidCastException::InvalidCastException( Exception* innerException )
	: InvalidCastException( "Cannot cast from source type to destination type.", innerException, "SC.InvalidCastException" )
{

}

InvalidCastException::InvalidCastException( String message, Exception* innerException )
	: InvalidCastException( message, innerException, "SC.InvalidCastException" )
{

}