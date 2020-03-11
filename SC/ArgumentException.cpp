using namespace SC;

ArgumentException::ArgumentException( String message, Exception* innerException, String name )
	: Exception( message, innerException, name )
{

}

ArgumentException::ArgumentException( String message, String parameterName, Exception* innerException, String name )
	: Exception( String::Format( "{0}\nParameter name: {1}", message, parameterName ), innerException, name )
	, parameterName( parameterName )
{

}

ArgumentException::ArgumentException( Exception* innerException )
	: ArgumentException( "An invalid argument was specified.", innerException, "SC.ArgumentException" )
{

}

ArgumentException::ArgumentException( String message, Exception* innerException )
	: ArgumentException( message, innerException, "SC.ArgumentException" )
{

}

ArgumentException::ArgumentException( String message, String parameterName, Exception* innerException )
	: ArgumentException( message, parameterName, innerException, "SC.ArgumentException" )
{

}

String ArgumentException::ParamName_get()
{
	return parameterName;
}