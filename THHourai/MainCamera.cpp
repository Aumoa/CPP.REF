using namespace Touhou;

using namespace std;

mutex MainCamera::locker;

MainCamera::MainCamera() : GameObject( "mainCamera" )
{
	AddComponent<Camera>();
}

RefPtr<MainCamera> MainCamera::GetInstance()
{
	lock_guard<mutex> lock( locker );
	static var obj = new MainCamera();
	return obj;
}