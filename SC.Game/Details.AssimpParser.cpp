using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

using namespace std;
using namespace std::filesystem;

XMFLOAT4& operator<<( XMFLOAT4& left, const aiColor4D& right )
{
	left.x = right.r;
	left.y = right.g;
	left.z = right.b;
	left.w = right.a;
	return left;
}

XMFLOAT3& operator<<( XMFLOAT3& left, const aiVector3D& right )
{
	left.x = right.x;
	left.y = right.y;
	left.z = right.z;
	return left;
}

XMFLOAT2& operator<<( XMFLOAT2& left, const aiVector3D& right )
{
	left.x = right.x;
	left.y = right.y;
	return left;
}

XMFLOAT4X4& operator<<( XMFLOAT4X4& left, const aiMatrix4x4& right )
{
	XMStoreFloat4x4( &left, XMMatrixTranspose( XMLoadFloat4x4( &( const XMFLOAT4X4& )right ) ) );
	//left = ( const XMFLOAT4X4& )right;
	return left;
}

Vector3& operator<<( Vector3& left, const aiVector3D& right )
{
	left.X = right.x;
	left.Y = right.y;
	left.Z = right.z;
	return left;
}

Quaternion& operator<<( Quaternion& left, const aiQuaternion& right )
{
	left.X = right.x;
	left.Y = right.y;
	left.Z = right.z;
	left.W = right.w;
	return left;
}

Vector3 ConvertVector3( FXMVECTOR right )
{
	return Vector3( ( double )XMVectorGetX( right ), ( double )XMVectorGetY( right ), ( double )XMVectorGetZ( right ) );
}

Quaternion ConvertQuaternion( FXMVECTOR right )
{
	return Quaternion( ( double )XMVectorGetX( right ), ( double )XMVectorGetY( right ), ( double )XMVectorGetZ( right ), ( double )XMVectorGetW( right ) );
}

float Average( aiColor3D right )
{
	return ( right.r + right.g + right.b ) / 3.0f;
}

AssimpParser::AssimpParser( const path& filepath, RefPtr<IAsyncLoad> asyncLoad, double beginProg, double endProg )
{
	auto str = filepath.string();
	scene = importer.ReadFile(
		str,
		aiProcess_LimitBoneWeights |
		aiProcess_Triangulate |
		aiProcess_MakeLeftHanded |
		aiProcess_FlipUVs
	);
	directory = filepath.parent_path();
}

RefPtr<GameObject> AssimpParser::TryParse( const wstring_view& name )
{
	RefPtr<GameObject> gameObject = new GameObject( name );

	ProcessMaterials( name, scene );
	ProcessMeshes( gameObject, scene );
	ProcessJointHierarchy( gameObject, scene->mRootNode );
	ProcessAnimations( name, scene );

	if ( !joints.empty() && controller )
	{
		gameObject->AddComponent<Animator>()->Controller = controller;
	}

	return gameObject;
}

void AssimpParser::ProcessMaterials( const wstring_view& name, const aiScene* scene )
{
	materials.resize( scene->mNumMaterials );

	for ( int i = 0; i < ( int )scene->mNumMaterials; ++i )
	{
		auto aiMat = scene->mMaterials[i];
		RefPtr mat = new Material( String::Format( "{0}:{1}", name, aiMat->GetName().C_Str() ) );

		if ( aiMat->GetTextureCount( aiTextureType_DIFFUSE ) > 0 )
		{
			aiString aiPath;
			aiMat->GetTexture( aiTextureType_DIFFUSE, 0, &aiPath );

			string pathOrg = aiPath.C_Str();
			size_t splitPos = pathOrg.find( '*' );

			string path1 = splitPos == string::npos ? pathOrg : pathOrg.substr( 0, splitPos );
			string path2 = splitPos == string::npos ? "" : pathOrg.substr( splitPos + 1 );

			if ( path1.empty() )
			{
				int embeded_index = stoi( path2 );
				auto texture = scene->mTextures[embeded_index];
				
				auto assetName = String::Format( "{0}:{1}_embeded{2}", name, aiMat->GetName().C_Str(), embeded_index );
				auto texture0 = AssetBundle::GetItemAs<Texture2D>( assetName );

				if ( !texture0 )
				{
					texture0 = new Texture2D( assetName, texture->pcData, texture->mWidth );
					mat->DiffuseMap = texture0;
					AssetBundle::AddItem( texture0 );
				}
			}
			else
			{
				auto assetName = String::Format( "{0}:{1}:DIFFUSE0", name, aiMat->GetName().C_Str() );

				try
				{
					auto texture0 = AssetBundle::GetItemAs<Texture2D>( path1 );
					if ( !texture0 )
					{
						texture0 = new Texture2D( String::Format( "{0}:{1}:DIFFUSE0", name, aiMat->GetName().C_Str() ), directory / path1 );
						AssetBundle::AddItem( texture0 );
					}
					mat->DiffuseMap = texture0;

					if ( splitPos != string::npos )
					{
						auto texture1 = AssetBundle::GetItemAs<Texture2D>( path2 );
						if ( !texture1 )
						{
							texture1 = new Texture2D( String::Format( "{0}:{1}:DIFFUSE1", name, aiMat->GetName().C_Str() ), directory / path2 );
							AssetBundle::AddItem( texture1 );
						}
						mat->DiffuseLayerMap = texture1;
					}
				}
				catch ( Exception * e )
				{
					delete e;
				}
			}
		}

		if ( aiMat->GetTextureCount( aiTextureType_NORMALS ) > 0 )
		{
			aiString aiPath;
			aiMat->GetTexture( aiTextureType_NORMALS, 0, &aiPath );

			string path = aiPath.C_Str();

			if ( path[0] == '*' )
			{
				int embeded_index = stoi( path.c_str() + 1 );
				auto texture = scene->mTextures[embeded_index];

				auto assetName = String::Format( "{0}:{1}_embeded{2}", name, aiMat->GetName().C_Str(), embeded_index );
				auto texture0 = AssetBundle::GetItemAs<Texture2D>( assetName );

				if ( !texture0 )
				{
					texture0 = new Texture2D( assetName, texture->pcData, texture->mWidth );
					mat->NormalMap = texture0;
					AssetBundle::AddItem( texture0 );
				}
			}
			else
			{
				auto assetName = String::Format( "{0}:{1}:DIFFUSE0", name, aiMat->GetName().C_Str() );

				try
				{
					auto allpath = directory / path;
					auto texture0 = AssetBundle::GetItemAs<Texture2D>( allpath.wstring() );
					if ( !texture0 )
					{
						texture0 = new Texture2D( String::Format( "{0}:{1}:DIFFUSE0", name, aiMat->GetName().C_Str() ), directory / path );
						AssetBundle::AddItem( texture0 );
					}
					mat->NormalMap = texture0;
				}
				catch ( Exception * e )
				{
					delete e;
				}
			}
		}

		aiColor3D ambientColor, diffuseColor, specularColor;
		float specExp;
		if ( aiMat->Get( AI_MATKEY_SHININESS, specExp ) != AI_SUCCESS ) specExp = 32;
		if ( aiMat->Get( AI_MATKEY_COLOR_AMBIENT, ambientColor ) != AI_SUCCESS ) ambientColor = aiColor3D( 1.0f, 1.0f, 1.0f );
		if ( aiMat->Get( AI_MATKEY_COLOR_DIFFUSE, diffuseColor ) != AI_SUCCESS ) diffuseColor = aiColor3D( 1.0f, 1.0f, 1.0f );
		if ( aiMat->Get( AI_MATKEY_COLOR_SPECULAR, specularColor ) != AI_SUCCESS ) specularColor = aiColor3D( 1.0f, 1.0f, 1.0f );

		mat->Ambient = Average( ambientColor );
		mat->Diffuse = Average( diffuseColor );
		mat->Specular = Average( specularColor );
		mat->SpecExp = ( double )specExp;

		materials[i] = mat;
	}
}

void AssimpParser::ProcessMeshes( RefPtr<GameObject> gameObject, const aiScene* scene )
{
	meshes.resize( scene->mNumMeshes );

	for ( int i = 0; i < ( int )scene->mNumMeshes; ++i )
	{
		auto mesh = scene->mMeshes[i];

		RefPtr<Mesh> m;

		if ( mesh->mNumBones != 0 )
		{
			vector<SkinnedVertex> vertexBuffer( mesh->mNumVertices );
			for ( int j = 0; j < ( int )mesh->mNumVertices; ++j )
			{
				auto& v = vertexBuffer[j];
				v = { };
				v.Color = { 1.0f, 1.0f, 1.0f, 1.0f };

				v.Pos << mesh->mVertices[j];
				if ( mesh->mNormals ) v.Normal << mesh->mNormals[j];
				if ( mesh->mTextureCoords[0] ) v.Tex << mesh->mTextureCoords[0][j];
				if ( mesh->mTangents ) v.Tangent << mesh->mTangents[j];
				if ( mesh->mColors[0] ) v.Color << mesh->mColors[0][j];
			}

			vector<uint32> indexBuffer( mesh->mNumFaces * 3 );
			for ( int j = 0; j < ( int )mesh->mNumFaces; ++j )
			{
				auto& faces = ( uint32( & )[3] )indexBuffer[j * 3];
				auto& face = mesh->mFaces[j];

#if defined( _DEBUG )
				if ( face.mNumIndices != 3 ) throw new Exception();
#endif

				faces[0] = face.mIndices[0];
				faces[1] = face.mIndices[1];
				faces[2] = face.mIndices[2];
			}

			auto jointStart = ( int )joints.size();
			joints.resize( jointStart + mesh->mNumBones );

			for ( int j = 0; j < ( int )mesh->mNumBones; ++j )
			{
				auto bone = mesh->mBones[j];
				auto jointId = jointStart + j;

				joints[jointId].Name = bone->mName.C_Str();
				joints[jointId].Offset << bone->mOffsetMatrix;

				for ( int i = 0; i < ( int )bone->mNumWeights; ++i )
				{
					auto& weights = bone->mWeights[i];
					auto& v = vertexBuffer[weights.mVertexId];
					AddJointIndex( v, jointId, weights.mWeight );
				}
			}

			m = new Mesh( String::Format( "{0}:{1}", gameObject->Name, mesh->mName.C_Str() ), vertexBuffer, indexBuffer );

			tag_Mesh tagm;
			tagm.pMesh = m;
			tagm.JointStart = jointStart;
			tagm.JointCount = ( int )mesh->mNumBones;

			meshes[i] = tagm;
		}
		else
		{
			vector<Vertex> vertexBuffer( mesh->mNumVertices );
			for ( int j = 0; j < ( int )mesh->mNumVertices; ++j )
			{
				auto& v = vertexBuffer[j];
				v = { };
				v.Color = { 1.0f, 1.0f, 1.0f, 1.0f };

				v.Pos << mesh->mVertices[j];
				if ( mesh->mNormals ) v.Normal << mesh->mNormals[j];
				if ( mesh->mTextureCoords[0] ) v.Tex << mesh->mTextureCoords[0][j];
				if ( mesh->mTangents ) v.Tangent << mesh->mTangents[j];
				if ( mesh->mColors[0] ) v.Color << mesh->mColors[0][j];
			}

			vector<uint32> indexBuffer( mesh->mNumFaces * 3 );
			for ( int j = 0; j < ( int )mesh->mNumFaces; ++j )
			{
				auto& faces = ( uint32( & )[3] )indexBuffer[j * 3];
				auto& face = mesh->mFaces[j];

#if defined( _DEBUG )
				if ( face.mNumIndices != 3 ) throw new Exception();
#endif

				faces[0] = face.mIndices[0];
				faces[1] = face.mIndices[1];
				faces[2] = face.mIndices[2];
			}

			m = new Mesh( String::Format( "{0}:{1}", gameObject->Name, mesh->mName.C_Str() ), vertexBuffer, indexBuffer );

			tag_Mesh tagm;
			tagm.pMesh = m;
			tagm.JointStart = 0;
			tagm.JointCount = 0;

			meshes[i] = tagm;
		}

		RefPtr go = new GameObject( mesh->mName.C_Str() );

		if ( mesh->mNumBones != 0 )
		{
			auto skinnedMeshRenderer = go->AddComponent<SkinnedMeshRenderer>();
			skinnedMeshRenderer->Mesh = m;
			skinnedMeshRenderer->Material = materials[mesh->mMaterialIndex];
		}
		else
		{
			go->AddComponent<MeshFilter>()->Mesh = m;
			go->AddComponent<MeshRenderer>()->Material = materials[mesh->mMaterialIndex];
		}

		go->Parent = gameObject;
	}
}

void AssimpParser::ProcessJointHierarchy( RefPtr<GameObject> gameObject, aiNode* node, int jointParent )
{
	string_view name = node->mName.C_Str();

	RefPtr<GameObject> go = new GameObject( name );

	auto jointIndex = SearchJoint( name );
	if ( jointIndex == -1 )
	{
		tag_Joint joint;
		joint.Name = name;
		joint.Offset << aiMatrix4x4( node->mTransformation ).Inverse();
		joint.Restore = true;
		
		jointIndex = ( int )joints.size();
		joints.push_back( joint );
	}

	auto& joint = joints[jointIndex];
	joint.Parent = jointParent;

	auto bone = go->AddComponent<Bone>();
	bone->Index = jointIndex;
	bone->Name = name;
	bone->Restore = joint.Restore;

	XMVECTOR pos, scale, quat;
	XMMatrixDecompose( &scale, &quat, &pos, XMLoadFloat4x4( &joint.Offset ) );

	auto trp = go->Transform;
	trp->Position = ConvertVector3( pos );
	trp->Scale = ConvertVector3( scale );
	trp->Rotation = ConvertQuaternion( quat );

	go->Parent = gameObject;

	for ( int i = 0; i < ( int )node->mNumChildren; ++i )
	{
		ProcessJointHierarchy( go, node->mChildren[i], jointIndex );
	}
}

void AssimpParser::ProcessAnimations( const std::wstring_view& name, const aiScene* scene )
{
	for ( int i = 0; i < ( int )scene->mNumAnimations; ++i )
	{
		auto animation = scene->mAnimations[i];
		auto tps = 1000.0;
		if ( tps == 0 ) tps = 24;
		RefPtr clip = new AnimationClip( String::Format( "{0}:{1}", name, animation->mName.C_Str() ) );

		for ( int i = 0; i < ( int )animation->mNumChannels; ++i )
		{
			auto channel = animation->mChannels[i];
			string bone = channel->mNodeName.C_Str();

			Keyframes keyframes;

			keyframes.Translation.resize( channel->mNumPositionKeys );
			for ( int i = 0; i < ( int )channel->mNumPositionKeys; ++i )
			{
				auto& vec = keyframes.Translation[i];
				auto key = channel->mPositionKeys[i];

				vec.TimePos = key.mTime / tps;
				vec.Value << key.mValue;
			}

			keyframes.Scaling.resize( channel->mNumScalingKeys );
			for ( int i = 0; i < ( int )channel->mNumScalingKeys; ++i )
			{
				auto& vec = keyframes.Scaling[i];
				auto key = channel->mScalingKeys[i];

				vec.TimePos = key.mTime / tps;
				vec.Value << key.mValue;
			}

			keyframes.Rotation.resize( channel->mNumRotationKeys );
			for ( int i = 0; i < ( int )channel->mNumRotationKeys; ++i )
			{
				auto& vec = keyframes.Rotation[i];
				auto key = channel->mRotationKeys[i];

				vec.TimePos = key.mTime / tps;
				vec.Value << key.mValue;
			}

			clip->Keyframes[bone] = move( keyframes );
		}

		AssetBundle::AddItem( clip );
		clip->IsLoop = true;

		RefPtr state = new AnimationState( animation->mName.C_Str() );
		state->Clip = clip;

		if ( i == 0 )
		{
			controller = new AnimatorController( String::Format( "{0}:DefaultController", name ) );
			controller->AddState( state, true );
		}
		else
		{
			controller->AddState( state );
		}
	}
}

void AssimpParser::AddJointIndex( SkinnedVertex& v, int jointIndex, float weight )
{
	if ( v.Weights[0] == 0 )
	{
		v.Indices[0] = jointIndex;
		v.Weights[0] = weight;
	}
	else if ( v.Weights[1] == 0 )
	{
		v.Indices[1] = jointIndex;
		v.Weights[1] = weight;
	}
	else if ( v.Weights[2] == 0 )
	{
		v.Indices[2] = jointIndex;
		v.Weights[2] = weight;
	}
	else
	{
		v.Indices[3] = jointIndex;
	}
}

int AssimpParser::SearchJoint( const string_view& name )
{
	for ( int i = 0, count = joints.size(); i < count; ++i )
	{
		if ( joints[i].Name == name ) return i;
	}
	return -1;
}