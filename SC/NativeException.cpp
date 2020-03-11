using namespace SC;

using namespace std;

NativeException::NativeException( exception_ptr native )
	: Exception( "Native exception detected.", nullptr, "SC.NativeException" )
	, native_ptr( native )
{

}

exception_ptr NativeException::NativePtr_get() const
{
	return native_ptr;
}