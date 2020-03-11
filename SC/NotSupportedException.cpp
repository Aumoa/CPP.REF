using namespace SC;

NotSupportedException::NotSupportedException( String message, Exception* innerException, String name )
	: Exception( message, innerException, name )
{

}

NotSupportedException::NotSupportedException( Exception* innerException )
	: NotSupportedException( "Specified method is not supported.", innerException )
{

}

NotSupportedException::NotSupportedException( String message, Exception* innerException )
	: NotSupportedException( message, innerException, "SC.NotSupportedException" )
{

}