using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

using namespace std;

void Animator::Render( RefPtr<CDeviceContext>& deviceContext )
{
	int fixedFrameIndex = GlobalVar.fixedFrameIndex;

	if ( boneTransform.size() )
	{
		if ( auto slot = deviceContext->Slot["BoneTransform"]; slot != -1 )
		{
			deviceContext->pCommandList->SetGraphicsRootShaderResourceView( slot, finalTransformBuffer[fixedFrameIndex]->VirtualAddress );
		}
	}
}

Animator::Animator() : Component()
{

}

Animator::~Animator()
{
	GC.Add( finalTransformBuffer );
}

object Animator::Clone()
{
	RefPtr clone = new Animator();

	clone->controller = controller;
	clone->animVars = animVars;

	clone->currentState = currentState;
	clone->prevState = prevState;
	clone->blendTime = blendTime;

	clone->boneTransform = boneTransform;

	return clone;
}

void Animator::Start()
{
	int numBones = ( int )Linked->GetComponentsInChildren<Bone>().size();

	boneTransform.resize( numBones );

	if ( numBones )
	{
		finalTransformBuffer[0] = GlobalVar.device->CreateDynamicBuffer( sizeof( tag_BoneTransform ) * numBones, -1 );
		finalTransformBuffer[1] = GlobalVar.device->CreateDynamicBuffer( sizeof( tag_BoneTransform ) * numBones, -1 );
		InitializeOffset( Linked );

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{ };
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Buffer.NumElements = numBones;
		srvDesc.Buffer.StructureByteStride = sizeof( tag_BoneTransform );
	}
	else
	{
		finalTransformBuffer[0] = nullptr;
		finalTransformBuffer[1] = nullptr;
	}
}

void Animator::FixedUpdate( Time& time )
{
	if ( controller && boneTransform.size() )
	{
		Transistor();

		currentState.Interpolate( time.FixedDeltaTimeInSeconds );
		prevState.Interpolate( time.FixedDeltaTimeInSeconds );
		UpdateBlend( time.FixedDeltaTimeInSeconds );

		UpdateToRoot( Linked );
		ReplaceToRoot();
	}
}

AnimVar Animator::Vars_get( const wstring_view& param0 )
{
	if ( auto it = animVars.find( param0 ); it != animVars.end() )
	{
		return it->second;
	}
	else
	{
		auto& params = controller->parameters;

		if ( auto it = params.find( param0 ); it != params.end() )
		{
			animVars.insert( { it->first, it->second } );
			return it->second;
		}
		else
		{
			return AnimVar( 0 );
		}
	}
}

void Animator::Vars_set( const wstring_view& param0, const AnimVar& value )
{
	if ( auto it = animVars.find( param0 ); it != animVars.end() )
	{
		it->second = value;
	}
	else
	{
		animVars.insert( { wstring( param0 ), value } );
	}
}

RefPtr<AnimatorController> Animator::Controller_get()
{
	return controller;
}

void Animator::Controller_set( RefPtr<AnimatorController> value )
{
	controller = value;
	animVars = value->MakeParameters();
	currentState = MakeStateMachine( value->DefaultState );
	prevState = { };
}

void Animator::InitializeOffset( RefPtr<GameObject> gameObject, int parent )
{
	auto bone = gameObject->GetComponent<Bone>();

	if ( bone )
	{
		auto idx = bone->Index;
		auto trp = gameObject->Transform;
		auto pos = trp->Position;
		auto scale = trp->Scale;
		auto quat = trp->Rotation;

		XMMATRIX offset =
			XMMatrixScaling( ( float )scale.X, ( float )scale.Y, ( float )scale.Z ) *
			XMMatrixRotationQuaternion( XMVectorSet( ( float )quat.X, ( float )quat.Y, ( float )quat.Z, ( float )quat.W ) ) *
			XMMatrixTranslation( ( float )pos.X, ( float )pos.Y, ( float )pos.Z );

		tag_BoneTransform* finalTransform[]
		{
			( tag_BoneTransform* )finalTransformBuffer[0]->pBlock,
			( tag_BoneTransform* )finalTransformBuffer[1]->pBlock
		};

		XMStoreFloat4x4( &boneTransform[idx].Offset, offset );
		XMStoreFloat4x4( &boneTransform[idx].ToRoot, XMMatrixIdentity() );
		XMStoreFloat4x4( &finalTransform[0][idx].World, XMMatrixIdentity() );
		XMStoreFloat4x4( &finalTransform[1][idx].World, XMMatrixIdentity() );

		parent = idx;
	}

	for ( int i = 0; i < gameObject->NumChilds; ++i )
	{
		InitializeOffset( gameObject->Childs[i], parent );
	}
}

void Animator::UpdateToRoot( RefPtr<GameObject> gameObject, int parent )
{
	auto bone = gameObject->GetComponent<Bone>();

	if ( bone )
	{
		auto idx = bone->Index;
		auto name = bone->Name;

		XMMATRIX transform = XMMatrixIdentity();

		auto interpolated = currentState.Keyframes.find( name );
		if ( interpolated != currentState.Keyframes.end() )
		{
			auto& key = interpolated->second;

			transform =
				XMMatrixScaling( ( float )key.Scaling.Value.X, ( float )key.Scaling.Value.Y, ( float )key.Scaling.Value.Z ) *
				XMMatrixRotationQuaternion( XMVectorSet( ( float )key.Rotation.Value.X, ( float )key.Rotation.Value.Y, ( float )key.Rotation.Value.Z, ( float )key.Rotation.Value.W ) ) *
				XMMatrixTranslation( ( float )key.Translation.Value.X, ( float )key.Translation.Value.Y, ( float )key.Translation.Value.Z );
		}

		XMMATRIX parentTransform = XMMatrixIdentity();

		if ( parent != -1 )
		{
			parentTransform = XMLoadFloat4x4( &boneTransform[parent].ToRoot );
		}

		XMMATRIX restore = XMMatrixIdentity();

		if ( bone->Restore )
		{
			XMMATRIX offset = XMLoadFloat4x4( &boneTransform[idx].Offset );
			XMVECTOR det = XMMatrixDeterminant( offset );
			restore = XMMatrixInverse( &det, offset );
		}

		XMStoreFloat4x4( &boneTransform[idx].ToRoot, transform * restore * parentTransform );
		parent = idx;
	}

	for ( int i = 0; i < gameObject->NumChilds; ++i )
	{
		UpdateToRoot( gameObject->Childs[i], parent );
	}
}

void Animator::ReplaceToRoot()
{
	int fixedFrameIndex = GlobalVar.fixedFrameIndex;

	for ( int i = 0, count = boneTransform.size(); i < count; ++i )
	{
		auto& bt = boneTransform[i];
		auto finalTransform = ( tag_BoneTransform* )finalTransformBuffer[fixedFrameIndex]->pBlock;

		XMMATRIX offset = XMLoadFloat4x4( &bt.Offset );
		XMMATRIX toRoot = XMLoadFloat4x4( &bt.ToRoot );

		XMMATRIX final = offset * toRoot;

		XMStoreFloat4x4( &finalTransform[i].World, final );
	}
}

void Animator::Transistor()
{
	auto conds = controller->Transition[currentState.Name];

	for ( auto i : conds )
	{
		bool change = false;
		bool checkIf = true;

		if ( i.HasExitTime )
		{
			if ( currentState.TimePos < ( currentState.Duration - i.BlendTime ) )
			{
				checkIf = false;
			}
		}

		if ( checkIf )
		{

			if ( i.VarName.empty() )
			{
				change = true;
			}
			else
			{
				auto var = Vars[i.VarName];
				if ( var.Type == 3 )
				{
					if ( var.Trigger )
					{
						Vars[i.VarName] = Trigger( false );
						change = true;
					}
				}
				else if ( i.Condition( var ) )
				{
					change = true;
				}
			}
		}

		if ( change )
		{
			if ( i.BlendTime != 0 )
			{
				prevState = currentState;
				blendTime = i.BlendTime;
			}

			currentState = MakeStateMachine( i.To );
			break;
		}
	}
}

void Animator::UpdateBlend( double timeDelta )
{
	if ( prevState.Clip )
	{
		if ( currentState.TimePos > blendTime )
		{
			prevState = { };
		}
		else
		{
			double t = currentState.TimePos / blendTime;

			for ( auto& i : currentState.Keyframes )
			{
				if ( auto keyt = prevState.Keyframes.find( i.first ); keyt != prevState.Keyframes.end() )
				{
					auto key1 = keyt->second;
					auto key2 = i.second;

					i.second = key1.Interpolate( key2, t );
				}
			}
		}
	}
}

auto Animator::MakeStateMachine( const wstring_view& name ) -> tag_StateMachine
{
	tag_StateMachine machine;

	machine.State = controller->States[name];
	if ( machine.State )
	{
		machine.Name = name;
		machine.Clip = machine.State->Clip;
		machine.Duration = machine.Clip->Duration;
		machine.Keyframes = machine.Clip->MakeInterpolateMap();
		machine.TimePos = 0;
	}
	else
	{
		return currentState;
	}

	return move( machine );
}