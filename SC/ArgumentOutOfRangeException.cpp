using namespace SC;

ArgumentOutOfRangeException::ArgumentOutOfRangeException( String message, Exception* innerException, String name )
	: ArgumentException( message, innerException, name )
{

}

ArgumentOutOfRangeException::ArgumentOutOfRangeException( Exception* innerException )
	: ArgumentOutOfRangeException( "Parameter is out of range.", innerException, "SC.ArgumentOutOfRangeException" )
{

}

ArgumentOutOfRangeException::ArgumentOutOfRangeException( String message, Exception* innerException )
	: ArgumentOutOfRangeException( message, innerException, "SC.ArgumentOutOfRangeException" )
{

}