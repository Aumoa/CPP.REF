using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;
using namespace SC::Drawing;

using namespace std;

void Camera::SetGraphicsRootConstantBufferView( RefPtr<CDeviceContext>& deviceContext )
{
	int frameIndex = GlobalVar.frameIndex;

	if ( auto slot = deviceContext->Slot["Camera"]; slot != -1 )
	{
		deviceContext->pCommandList->SetGraphicsRootConstantBufferView( ( UINT )slot, dynamicBuffer[frameIndex]->VirtualAddress );
	}
}

Camera::Camera()
{
	dynamicBuffer[0] = GlobalVar.device->CreateDynamicBuffer( sizeof( Constants ), 256 );
	dynamicBuffer[1] = GlobalVar.device->CreateDynamicBuffer( sizeof( Constants ), 256 );
}

Camera::~Camera()
{
	GC.Add( dynamicBuffer );
}

object Camera::Clone()
{
	RefPtr ptr = new Camera();
	ptr->clearMode = clearMode;
	ptr->aspectRatio = aspectRatio;
	return ptr;
}

void Camera::LateUpdate( Time& time, Input& input )
{
	int frameIndex = GlobalVar.frameIndex;
	auto& transform = *( Transform::Constants* )Transform->dynamicBuffer[frameIndex]->pBlock;
	auto& frameResource = *( Constants* )dynamicBuffer[frameIndex]->pBlock;

	auto world = XMLoadFloat4x4( &transform.World );
	auto det = XMMatrixDeterminant( world );
	auto worldInv = XMMatrixInverse( &det, world );

	double asp = aspectRatio;
	if ( asp == 0.0 )
	{
		RECT rc;
		GetClientRect( GlobalVar.hWnd, &rc );
		auto wid = rc.right - rc.left;
		auto hei = rc.bottom - rc.top;
		asp = ( double )wid / hei;
	}

	auto view = worldInv;
	auto proj = XMMatrixPerspectiveFovLH( 0.25f * 3.14f, ( float )asp, 0.1f, 1000.0f );
	auto detProj = XMMatrixDeterminant( proj );

	auto vp = view * proj;

	XMStoreFloat4x4( &frameResource.ViewProj, vp );
	XMStoreFloat4x4( &frameResource.ViewInv, world );
	XMStoreFloat4x4( &frameResource.ProjInv, XMMatrixInverse( &detProj, proj ) );
	
	XMVECTOR scale, rotation, trans;
	XMMatrixDecompose( &scale, &rotation, &trans, world );

	XMStoreFloat3( &frameResource.Pos, trans );
}

Ray Camera::ScreenSpaceToRay( Point<double> screenPoint )
{
	int frameIndex = GlobalVar.frameIndex;
	auto& transform = *( Transform::Constants* )Transform->dynamicBuffer[frameIndex]->pBlock;

	auto world = XMLoadFloat4x4( &transform.World );
	auto det = XMMatrixDeterminant( world );
	auto worldInv = XMMatrixInverse( &det, world );

	double asp = aspectRatio;
	if ( asp == 0.0 )
	{
		RECT rc;
		GetClientRect( GlobalVar.hWnd, &rc );
		auto wid = rc.right - rc.left;
		auto hei = rc.bottom - rc.top;
		asp = ( double )wid / hei;
	}

	auto view_ = worldInv;
	auto proj_ = XMMatrixPerspectiveFovLH( 0.25f * 3.14f, ( float )asp, 0.1f, 1000.0f );

	XMFLOAT4X4 viewInv;
	XMFLOAT4X4 proj;
	XMStoreFloat4x4( &proj, proj_ );
	det = XMMatrixDeterminant( view_ );
	auto inv = XMMatrixInverse( &det, view_ );
	XMStoreFloat4x4( &viewInv, inv );

	Vector3 viewSpace;
	viewSpace.X = ( screenPoint.X - proj._31 ) / proj._11;
	viewSpace.Y = ( screenPoint.Y - proj._32 ) / proj._22;
	viewSpace.Z = 1.0;

	Vector3 dir;
	Vector3 org;

	dir.X = viewSpace.X * viewInv._11 + viewSpace.Y * viewInv._21 + viewSpace.Z * viewInv._31;
	dir.Y = viewSpace.X * viewInv._12 + viewSpace.Y * viewInv._22 + viewSpace.Z * viewInv._32;
	dir.Z = viewSpace.X * viewInv._13 + viewSpace.Y * viewInv._23 + viewSpace.Z * viewInv._33;

	org.X = viewInv._41;
	org.Y = viewInv._42;
	org.Z = viewInv._43;

	Ray ray;
	ray.Origin = org;
	ray.Direction = dir.Normalized;
	ray.MinDepth = 0.1;
	ray.MaxDepth = 1000.0;

	return ray;
}

RefPtr<CameraClearMode> Camera::ClearMode_get()
{
	return clearMode;
}

void Camera::ClearMode_set( RefPtr<CameraClearMode> value )
{
	clearMode = value;
}

double Camera::AspectRatio_get()
{
	return aspectRatio;
}

void Camera::AspectRatio_set( double value )
{
	aspectRatio = value;
}