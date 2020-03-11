using namespace SC;

ArgumentNullException::ArgumentNullException( String message, Exception* innerException, String name )
	: ArgumentException( message, innerException, name )
{

}

ArgumentNullException::ArgumentNullException( String message, String parameterName, Exception* innerException, String name )
	: ArgumentException( String::Format( "{0}\nParameter name: {1}", message, parameterName ), innerException, name )
{

}

ArgumentNullException::ArgumentNullException( Exception* innerException )
	: ArgumentNullException( "Value cannot be null.", innerException, "SC.ArgumentNullException" )
{

}

ArgumentNullException::ArgumentNullException( String message, Exception* innerException )
	: ArgumentNullException( message, innerException, "SC.ArgumentNullException" )
{

}

ArgumentNullException::ArgumentNullException( String message, String parameterName, Exception* innerException )
	: ArgumentNullException( message, parameterName, innerException, "SC.ArgumentNullException" )
{

}