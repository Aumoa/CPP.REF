using namespace SC;

NotImplementedException::NotImplementedException( String message, Exception* innerException, String name )
	: Exception( message, innerException, name )
{

}

NotImplementedException::NotImplementedException( Exception* innerException )
	: NotImplementedException( "A requested method or operation is not implemented.", innerException )
{

}

NotImplementedException::NotImplementedException( String message, Exception* innerException )
	: NotImplementedException( message, innerException, "SC.NotImplementedException" )
{

}