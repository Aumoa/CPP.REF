using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;
using namespace SC::Threading;

using namespace std;
using namespace std::filesystem;

multimap<String, RefPtr<Assets>> AssetBundle::assets;

inline RefPtr<Assets> getter( object iter )
{
	auto it = iter.As<multimap<String, RefPtr<Assets>>::iterator>();
	return it->second;
}

RefPtr<IEnumerator<RefPtr<Assets>>> AssetBundle::GetEnumerator()
{
	return new BasicEnumerator<RefPtr<Assets>>( assets.begin(), assets.end(), getter );
}

void AssetBundle::AddItem( RefPtr<Assets> value )
{
	assets.insert( { value->Name, value } );
}

RefPtr<Assets> AssetBundle::GetItem( String name )
{
	auto it = assets.find( name );
	if ( it != assets.end() ) return it->second;
	else return nullptr;
}

RefPtr<IEnumerator<RefPtr<Assets>>> AssetBundle::GetItems( String name )
{
	return new BasicEnumerator<RefPtr<Assets>>( assets.lower_bound( name ), assets.upper_bound( name ), getter );
}

bool AssetBundle::Contains( String name )
{
	auto it = assets.find( name );
	return it != assets.end();
}

RefPtr<IAsyncLoad> AssetBundle::LoadModelAssetsAsync( const path& filepath )
{
	RefPtr asyncLoad = new AsyncLoad();
	asyncLoad->IsLocked = true;
	AsyncLoad* ptr = asyncLoad.Get();
	asyncLoad->Bind( ThreadPool::CreateTask( [ptr]( object arg ) -> object
		{
			RefPtr asyncLoad_ = ptr;
			asyncLoad_->IsLocked = false;
			return CreateGameObjectFromModel( arg.As<path>(), asyncLoad_ );
		}, filepath
	) );
	return asyncLoad;
}

RefPtr<GameObject> AssetBundle::CreateGameObjectFromModel( const path& filepath, RefPtr<IAsyncLoad> asyncLoad )
{
	path ext = filepath.extension();

	if ( ext == ".m3d" )
	{
		auto ptr = CreateGameObjectFromM3DModel( filepath, asyncLoad );
		return ptr;
	}
	else if ( ext == ".mdl" )
	{
		auto ptr = CreateGameObjectFromMDLModel( filepath, asyncLoad );
		return ptr;
	}
	else
	{
		auto ptr = CreateGameObjectFromAssimpModel( filepath, asyncLoad );
		return ptr;
	}
}

inline namespace
{
	namespace M3D
	{
		inline XMFLOAT2& operator<<( XMFLOAT2& out, float* vec )
		{
			return out = { vec[0], vec[1] };
		}

		inline XMFLOAT3& operator<<( XMFLOAT3& out, float* vec )
		{
			return out = { vec[0], vec[1], vec[2] };
		}
	}
}

RefPtr<GameObject> AssetBundle::CreateGameObjectFromM3DModel( const path& filepath, RefPtr<IAsyncLoad> asyncLoad )
{
	using namespace M3D;

	auto head = AssetBundle::GetItemAs<GameObject>( filepath.c_str() );
	if ( !head )
	{
		RefPtr parser = new M3DParser( filepath, asyncLoad, 0, 0.8 );

		auto& header = parser->Header;
		auto& boneOffsets = parser->BoneOffsets;
		auto& hierarchy = parser->BoneHierarchy;
		auto& subsets = parser->Subsets;
		auto& materials = parser->Materials;
		auto& vertices = parser->Vertices;
		auto& triangles = parser->Triangles;
		auto& animationClips = parser->AnimationClips;

		head = new GameObject( filepath.filename().c_str() );

		vector<RefPtr<GameObject>> boneObjects( header.Bones );
		for ( int i = 0; i < header.Bones; ++i )
		{
			XMVECTOR pos;
			XMVECTOR scale;
			XMVECTOR quat;

			XMMatrixDecompose( &scale, &quat, &pos, XMLoadFloat4x4( &boneOffsets[i].BoneOffset ) );
			RefPtr gameObject = new GameObject( String::Format( "Bone{0}", i ) );
			auto trp = gameObject->Transform;
			trp->Position = Vector3( XMVectorGetX( pos ), XMVectorGetY( pos ), XMVectorGetZ( pos ) );
			trp->Scale = Vector3( XMVectorGetX( scale ), XMVectorGetY( scale ), XMVectorGetZ( scale ) );
			trp->Rotation = Quaternion( XMVectorGetX( quat ), XMVectorGetY( quat ), XMVectorGetZ( quat ), XMVectorGetW( quat ) );

			int parent = hierarchy[i];
			if ( parent == -1 )
			{
				gameObject->Parent = head;
			}
			else
			{
				gameObject->Parent = boneObjects[parent];
			}

			auto bone = gameObject->AddComponent<Bone>();
			bone->Name = "Bone" + to_string( i );
			bone->Index = i;
			boneObjects[i] = gameObject;
		}

		for ( int i = 0; i < header.Materials; ++i )
		{
			auto& subset = subsets[i];

			RefPtr material = new Material( String::Format( "{0}_subset{1}_material", filepath, i ) );
			AssetBundle::AddItem( material );

			auto& mat = materials[i];
			material->Ambient = ( double )mat.Ambient[0];
			material->Diffuse = ( double )mat.Diffuse[0];
			material->Specular = ( double )mat.Specular[0];
			material->SpecExp = ( double )mat.SpecPower;

			RefPtr diffuseMap = new Texture2D( String::Format( "{0}_subset{1}_diffuseMap", filepath, i ), filepath.parent_path() / mat.DiffuseMap );
			AssetBundle::AddItem( diffuseMap );
			material->DiffuseMap = diffuseMap;

			RefPtr<GameObject> gameObject;

			if ( header.IsSkinnedMesh )
			{
				vector<SkinnedVertex> vertexBuffer( subset.VertexCount );
				vector<uint32> indexBuffer( subset.FaceCount * 3 );

				for ( int i = 0; i < subset.VertexCount; ++i )
				{
					int vIndex = subset.VertexStart + i;

					vertexBuffer[i].Pos << vertices[vIndex].Position;
					vertexBuffer[i].Color = { 1.0f, 1.0f, 1.0f, 1.0f };
					vertexBuffer[i].Normal << vertices[vIndex].Normal;
					vertexBuffer[i].Tex << vertices[vIndex].TexCoords;
					vertexBuffer[i].Tangent << vertices[vIndex].Tangent;
					memcpy( vertexBuffer[i].Weights,vertices[vIndex].BlendWeights, sizeof( float[3] ) );
					vertexBuffer[i].Indices[0] = vertices[vIndex].BlendIndices[0];
					vertexBuffer[i].Indices[1] = vertices[vIndex].BlendIndices[1];
					vertexBuffer[i].Indices[2] = vertices[vIndex].BlendIndices[2];
					vertexBuffer[i].Indices[3] = vertices[vIndex].BlendIndices[3];
				}

				for ( int i = 0; i < subset.FaceCount; ++i )
				{
					int fIndex = subset.FaceStart + i;
					auto& face = triangles[fIndex];

					indexBuffer[i * 3 + 0] = face.Face[2] - subset.VertexStart;
					indexBuffer[i * 3 + 1] = face.Face[1] - subset.VertexStart;
					indexBuffer[i * 3 + 2] = face.Face[0] - subset.VertexStart;
				}

				RefPtr mesh = new Mesh( String::Format( "{0}_subset{1}_mesh", filepath, i ), vertexBuffer, indexBuffer );
				AssetBundle::AddItem( mesh );

				gameObject = new GameObject( String::Format( "{0}_subset{1}_gameObject", filepath, i ) );
				auto meshRenderer = gameObject->AddComponent<SkinnedMeshRenderer>();
				meshRenderer->Material = material;
				meshRenderer->Mesh = mesh;
			}
			else
			{
				vector<Vertex> vertexBuffer( subset.VertexCount );
				vector<uint32> indexBuffer( subset.FaceCount * 3 );

				for ( int i = 0; i < subset.VertexCount; ++i )
				{
					int vIndex = subset.VertexStart + i;

					vertexBuffer[i].Pos << vertices[vIndex].Position;
					vertexBuffer[i].Color = { 1.0f, 1.0f, 1.0f, 1.0f };
					vertexBuffer[i].Normal << vertices[vIndex].Normal;
					vertexBuffer[i].Tex << vertices[vIndex].TexCoords;
					vertexBuffer[i].Tangent << vertices[vIndex].Tangent;
				}

				for ( int i = 0; i < subset.FaceCount; ++i )
				{
					int fIndex = subset.FaceStart + i;
					auto& face = triangles[fIndex];

					indexBuffer[i * 3 + 0] = face.Face[2] - subset.VertexStart;
					indexBuffer[i * 3 + 1] = face.Face[1] - subset.VertexStart;
					indexBuffer[i * 3 + 2] = face.Face[0] - subset.VertexStart;
				}

				RefPtr mesh = new Mesh( String::Format( "{0}_subset{1}_mesh", filepath, i ), vertexBuffer, indexBuffer );
				AssetBundle::AddItem( mesh );

				gameObject = new GameObject( String::Format( "{0}_subset{1}_gameObject", filepath, i ) );
				auto meshFilter = gameObject->AddComponent<MeshFilter>();
				auto meshRenderer = gameObject->AddComponent<MeshRenderer>();
				meshFilter->Mesh = mesh;
				meshRenderer->Material = material;
			}

			gameObject->Parent = head;
		}

		for ( int i = 0; i < header.AnimationClips; ++i )
		{
			auto& clip = animationClips[i];

			RefPtr ptr = new AnimationClip( String::Format( "{0}:{1}", filepath.c_str(), clip.Name ) );

			for ( int i = 0; i < header.Bones; ++i )
			{
				auto& keyframes = clip.BoneKeyframes[i];

				Keyframes keyf;
				auto boneName = "Bone"s + to_string( i );

				keyf.Translation.resize( keyframes.NumKeyframes );
				keyf.Scaling.resize( keyframes.NumKeyframes );
				keyf.Rotation.resize( keyframes.NumKeyframes );

				for ( int i = 0; i < keyframes.NumKeyframes; ++i )
				{
					auto& keyframe = keyframes.Keyframes[i];

					keyf.Translation[i].TimePos = ( double )keyframe.Time;
					keyf.Scaling[i].TimePos = ( double )keyframe.Time;
					keyf.Rotation[i].TimePos = ( double )keyframe.Time;

					keyf.Translation[i].Value.X = keyframe.Pos[0];
					keyf.Translation[i].Value.Y = keyframe.Pos[1];
					keyf.Translation[i].Value.Z = keyframe.Pos[2];

					keyf.Scaling[i].Value.X = keyframe.Scale[0];
					keyf.Scaling[i].Value.Y = keyframe.Scale[1];
					keyf.Scaling[i].Value.Z = keyframe.Scale[2];

					keyf.Rotation[i].Value.X = keyframe.Quat[0];
					keyf.Rotation[i].Value.Y = keyframe.Quat[1];
					keyf.Rotation[i].Value.Z = keyframe.Quat[2];
					keyf.Rotation[i].Value.W = keyframe.Quat[3];
				}

				ptr->Keyframes[boneName] = keyf;
			}

			AddItem( ptr );
		}

		if ( !boneObjects.empty() )
		{
			head->AddComponent<Animator>();
		}
	}

	RefPtr gameObject = head->Clone().As<GameObject>();
	return gameObject;
}

RefPtr<GameObject> AssetBundle::CreateGameObjectFromMDLModel( const path& filepath, RefPtr<IAsyncLoad> asyncLoad )
{
	auto wstr = filepath.filename().wstring();
	auto head = AssetBundle::GetItemAs<GameObject>( wstr );
	if ( !head )
	{
		RefPtr parser = new MDLParser( filepath, asyncLoad, 0, 0.8 );
		head = parser->CreateGameObject( wstr.c_str(), filepath.parent_path() );
		if ( parser->sequences.size() != 0 )
		{
			auto& sequences = parser->sequences;
			auto& bones = parser->bones;
			auto& pivots = parser->pivots;

			int numAnimationClips = ( int )sequences.size();
			int numBones = ( int )bones.size();
			vector<RefPtr<AnimationClip>> clips( numAnimationClips );

			for ( int i = 0; i < numAnimationClips; ++i )
			{
				RefPtr clip = new AnimationClip( String::Format( "{0}:{1}", wstr, sequences[i].Anim ) );

				auto& seq = sequences[i];

				for ( int i = 0; i < numBones; ++i )
				{
					auto& bone = bones[i];

					Keyframes keyf;
					auto boneName = bones[i].Name;

					MDLParser::tag_Keyframe<Vector3> translation;
					MDLParser::tag_Keyframe<Vector3> scale;
					MDLParser::tag_Keyframe<Quaternion> rotation;
					bone.Export( seq.Interval, translation, scale, rotation );

					keyf.Translation.resize( translation.Keyframes.size() );
					keyf.Scaling.resize( scale.Keyframes.size() );
					keyf.Rotation.resize( rotation.Keyframes.size() );

					Vector3 pivotdel = pivots[i];
					if ( bone.Parent != -1 )
					{
						pivotdel -= pivots[bone.Parent];
					}

					pivotdel = Vector3( -pivotdel.Y, pivotdel.Z, pivotdel.X );

					if ( keyf.Translation.empty() )
					{
						keyf.Translation.resize( 1 );
						keyf.Translation[0].TimePos = 0;
						keyf.Translation[0].Value = pivotdel;
					}
					else
					{
						for ( int i = 0; i < ( int )keyf.Translation.size(); ++i )
						{
							auto tp = ( double )( translation.Keyframes[i].IntervalPos - seq.Interval.Begin ) / 1000.0;
							auto value = translation.Keyframes[i].Value;
							value = Vector3( -value.Y, value.Z, value.X ) + pivotdel;
							keyf.Translation[i].TimePos = ( float )tp;
							keyf.Translation[i].Value = value;
						}
					}

					for ( int i = 0; i < ( int )keyf.Scaling.size(); ++i )
					{
						auto tp = ( double )( scale.Keyframes[i].IntervalPos - seq.Interval.Begin ) / 1000.0;
						keyf.Scaling[i].TimePos = ( float )tp;
						auto value = scale.Keyframes[i].Value;
						value = Vector3( -value.Y, value.Z, value.X );
						keyf.Scaling[i].Value = value;
					}

					for ( int i = 0; i < ( int )keyf.Rotation.size(); ++i )
					{
						auto tp = ( double )( rotation.Keyframes[i].IntervalPos - seq.Interval.Begin ) / 1000.0;

						auto rot = rotation.Keyframes[i].Value;
						rot = Quaternion( -rot.Y, rot.Z, rot.X, -rot.W );

						keyf.Rotation[i].TimePos = ( float )tp;
						keyf.Rotation[i].Value = rot;
					}

					clip->Keyframes[boneName] = keyf;
				}

				AddItem( clip );
				clip->IsLoop = !seq.NonLooping;

				clips[i] = clip;
			}

			if ( numAnimationClips != 0 )
			{
				RefPtr animatorController = new AnimatorController( String::Format( "{0}:AnimatorController", filepath.c_str() ) );
				
				for ( int i = 0; i < numAnimationClips; ++i )
				{
					RefPtr ptr = new AnimationState( sequences[i].Anim );
					ptr->Clip = clips[i];
					animatorController->AddState( ptr, i == 0 );
				}

				auto animator = head->AddComponent<Animator>();
				animator->Controller = animatorController;
			}
		}

		AddItem( head );
	}

	RefPtr gameObject = head->Clone().As<GameObject>();
	asyncLoad->Progress = 1.0;
	return gameObject;
}

RefPtr<GameObject> AssetBundle::CreateGameObjectFromAssimpModel( const path& filepath, RefPtr<IAsyncLoad> asyncLoad )
{
	auto filename = filepath.filename().wstring();

	auto head = GetItemAs<GameObject>( filepath.wstring() );
	if ( !head )
	{
		AssimpParser ap( filepath, asyncLoad, 0, 0.9 );
		head = ap.TryParse( filename );
	}

	asyncLoad->Progress = 1.0;
	return head->Clone().As<GameObject>();
}