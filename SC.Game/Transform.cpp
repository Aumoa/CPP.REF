using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

void Transform::SetGraphicsRootConstantBufferView( RefPtr<CDeviceContext>& deviceContext )
{
	int frameIndex = GlobalVar.frameIndex;

	if ( auto slot = deviceContext->Slot["World"]; slot != -1 )
	{
		deviceContext->pCommandList->SetGraphicsRootConstantBufferView( ( UINT )slot, dynamicBuffer[frameIndex]->VirtualAddress );
	}
}

Transform::Transform()
{
	scale = Vector3::One;

	dynamicBuffer[0] = GlobalVar.device->CreateDynamicBuffer( sizeof( Constants ), 256 );
	dynamicBuffer[1] = GlobalVar.device->CreateDynamicBuffer( sizeof( Constants ), 256 );
}

Transform::~Transform()
{
	GC.Add( dynamicBuffer );
}

object Transform::Clone()
{
	RefPtr trans = new Transform();
	trans->position = position;
	trans->scale = scale;
	trans->rotation = rotation;
	return trans;
}

void Transform::LateUpdate( Time& time, Input& input )
{
	int frameIndex = GlobalVar.frameIndex;
	auto& frameResource = *( Constants* )dynamicBuffer[frameIndex]->pBlock;

	XMMATRIX worldPar = XMMatrixIdentity();

	if ( Linked->Parent )
	{
		auto& trp = *( Constants* )Linked->Parent->Transform->dynamicBuffer[frameIndex]->pBlock;
		worldPar = XMLoadFloat4x4( &trp.World );
	}

	XMVECTOR pos = XMVectorSet( ( float )position.X, ( float )position.Y, ( float )position.Z, 0 );
	XMVECTOR scale = XMVectorSet( ( float )this->scale.X, ( float )this->scale.Y, ( float )this->scale.Z, 0 );
	XMVECTOR quat = XMVectorSet( ( float )rotation.X, ( float )rotation.Y, ( float )rotation.Z, ( float )rotation.W );

	XMMATRIX world =
		XMMatrixScalingFromVector( scale ) *
		XMMatrixRotationQuaternion( quat ) *
		XMMatrixTranslationFromVector( pos );

	XMMATRIX worldFinal = world * worldPar;
	XMVECTOR deter = XMMatrixDeterminant( worldFinal );
	XMStoreFloat4x4( &frameResource.World, worldFinal );
	XMStoreFloat4x4( &frameResource.WorldInvTranspose, XMMatrixTranspose( XMMatrixInverse( &deter, worldFinal ) ) );
}

void Transform::LookAt( RefPtr<Transform> target, Vector3 up )
{
	LookAt( target->Position, up );
}

void Transform::LookAt( Vector3 target, Vector3 up )
{
	auto dir = target - Position;
	Rotation = Quaternion::LookTo( dir, up );
}

void Transform::LookTo( Vector3 direction, Vector3 up )
{
	Rotation = Quaternion::LookTo( direction, up );
}

Vector3 Transform::Position_get()
{
	return position;
}

void Transform::Position_set( Vector3 value )
{
	position = value;
}

Vector3 Transform::Scale_get()
{
	return scale;
}

void Transform::Scale_set( Vector3 value )
{
	scale = value;
}

Quaternion Transform::Rotation_get()
{
	return rotation;
}

void Transform::Rotation_set( Quaternion value )
{
	rotation = value;
}

Vector3 Transform::Forward_get()
{
	return ( rotation * Quaternion( Vector3::Forward, 0 ) * rotation.Conjugate ).V;
}