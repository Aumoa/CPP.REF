using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

using namespace std;
using namespace std::filesystem;

MDLParser::MDLParser( const char* block, int length, RefPtr<IAsyncLoad> asyncLoad, double beginProg, double endProg ) : TextParser( " \t,\n\r" )
	, asyncLoad( asyncLoad )
	, beginProg( beginProg )
	, endProg( endProg )
{
	TryParse();
}

MDLParser::MDLParser( path filepath, RefPtr<IAsyncLoad> asyncLoad, double beginProg, double endProg ) : TextParser( " \t,\n\r" )
	, asyncLoad( asyncLoad )
	, beginProg( beginProg )
	, endProg( endProg )
{
	ifstream file( filepath );

	if ( file.is_open() == false ) throw new Exception();

	size_t filesize = ( size_t )file.seekg( 0, ios::end ).tellg();
	file.seekg( 0, ios::beg );

	std::vector<char> buffer( filesize );
	int bufferSeek = 0;

	Progress = 0;

	double progressSteprate = 1024.0 / ( double )filesize * 0.2;
	bool cond = true;
	while ( cond )
	{
		int readlen = 1024;
		if ( filesize <= 1024 )
		{
			readlen = ( int )filesize;
			cond = false;
		}
		file.read( buffer.data() + bufferSeek, readlen );
		Progress += progressSteprate;
		bufferSeek += readlen;
		filesize -= readlen;
	}

	file.close();

	SetBuffer( buffer.data() );
	TryParse();
}

RefPtr<GameObject> MDLParser::CreateGameObject( String name, const path& resourcePath )
{
	RefPtr head = new GameObject( name );

	auto textures = LoadTextures( name, resourcePath );
	auto materials = LoadMaterials( name, textures );
	LoadBones( name, head );
	LoadMeshes( name, head, materials );

	return head;
}

void MDLParser::TryParse()
{
	while ( !Eof )
	{
		auto context = ReadString();

		if ( context == "Version" )
		{
			ReadVersion();
		}
		else if ( context == "Model" )
		{
			ReadModel();
		}
		else if ( context == "Sequences" )
		{
			int numSeqs = ReadInt();
			ReadChar();  // {

			sequences.resize( numSeqs );
			for ( int i = 0; i < numSeqs; ++i )
			{
				sequences[i] = ReadSequence();
			}
			ReadChar();  // }
		}
		else if ( context == "Textures" )
		{
			int numTextures = ReadInt();
			ReadChar();  // {
			textures.resize( numTextures );

			for ( int i = 0; i < numTextures; ++i )
			{
				textures[i] = move( ReadTexture() );
			}
		}
		else if ( context == "Materials" )
		{
			int numMaterials = ReadInt();
			materials.resize( numMaterials );
			ReadChar();  // {
			for ( int i = 0; i < numMaterials; ++i )
			{
				ReadString();
				materials[i] = move( ReadMaterial() );
			}
		}
		else if ( context == "Geoset" )
		{
			double geosetProg = 0.8 / model.NumGeosets;
			geosets.push_back( move( ReadGeoset() ) );
			Progress += geosetProg;
		}
		else if ( context == "GeosetAnim" )
		{
			ReadChar();
			ReadGeosetAnim();
		}
		else if ( context == "Bone" )
		{
			ReadBone();
		}
		else if ( context == "Helper" )
		{
			ReadBone();
		}
		else if ( context == "Attachment" )
		{
			ReadBone();
		}
		else if ( context == "PivotPoints" )
		{
			int numPivots = ReadInt();
			pivots.resize( numPivots );

			ReadChar();  // {
			for ( int i = 0; i < numPivots; ++i )
			{
				pivots[i] = ReadVector3InBracket();
			}
			ReadChar();  // }
		}
		else if ( context == "Camera" )
		{
			ReadString();
			ForEachContextsInBracket();
		}
		else if ( context == "CollisionShape" )
		{
			ReadString();
			ForEachContextsInBracket();
		}
	}

	Progress = 1;
}

void MDLParser::ReadVersion()
{
	ForEachContextsInBracket( [&]( string context )
		{
			if ( context == "FormatVersion" )
			{
				version.FormatVersion = ReadInt();
			}
		}
	);
}

void MDLParser::ReadModel()
{
	model.Name = ReadString();

	ForEachContextsInBracket( [&]( string context )
		{
			if ( context == "NumGeosets" )
			{
				model.NumGeosets = ReadInt();
			}
			else if ( context == "NumGeosetAnims" )
			{
				model.NumGeosets = ReadInt();
			}
			else if ( context == "NumHelpers" )
			{
				model.NumHelpers = ReadInt();
			}
			else if ( context == "NumBones" )
			{
				model.NumBones = ReadInt();
			}
			else if ( context == "NumAttachments" )
			{
				model.NumAttachments = ReadInt();
			}
			else if ( context == "BlendTime" )
			{
				model.BlendTime = ReadInt();
			}
			else if ( context == "MinimumExtent" )
			{
				model.MinimumExtent = ReadVector3InBracket();
			}
			else if ( context == "MaximumExtent" )
			{
				model.MaximumExtent = ReadVector3InBracket();
			}
		}
	);

	geosetAnims.resize( model.NumGeosets );
}

auto MDLParser::ReadGeoset() -> tag_Geoset
{
	tag_Geoset geoset;

	ForEachContextsInBracket( [&]( string context )
		{
			if ( context == "Vertices" )
			{
				int numVertex = ReadInt();
				geoset.Vertices.resize( numVertex );
				ReadChar();  // {
				{
					for ( int i = 0; i < numVertex; ++i )
					{
						geoset.Vertices[i] = ReadVector3InBracket();
					}
				}
				ReadChar();  // }
			}
			else if ( context == "Normals" )
			{
				int numNormal = ReadInt();
				geoset.Normals.resize( numNormal );
				ReadChar();  // {
				{
					for ( int i = 0; i < numNormal; ++i )
					{
						geoset.Normals[i] = ReadVector3InBracket();
					}
				}
				ReadChar();  // }
			}
			else if ( context == "TVertices" )
			{
				int numTVs = ReadInt();
				geoset.TVertices.resize( numTVs );
				ReadChar();  // {
				{
					for ( int i = 0; i < numTVs; ++i )
					{
						geoset.TVertices[i] = ReadVector2InBracket();
					}
				}
				ReadChar();  // }
			}
			else if ( context == "VertexGroup" )
			{
				ReadChar();  // {
				geoset.VertexGroup.resize( geoset.Vertices.size() );
				for ( int i = 0, count = ( int )geoset.Vertices.size(); i < count; ++i )
				{
					geoset.VertexGroup[i] = ReadInt();
				}
				ReadChar();  // }
			}
			else if ( context == "Faces" )
			{
				auto numTriangles = ReadInt();
				auto numIndexes = ReadInt();
				geoset.Faces.resize( numTriangles );

				ReadChar();  // {
				for ( int i = 0; i < numTriangles; ++i )
				{
					ReadString(); ReadChar();  // {
					geoset.Faces[i] = move( ReadTriangles( numIndexes ) );
					ReadChar();  // }
				}
				ReadChar();  // }
			}
			else if ( context == "Groups" )
			{
				int numGroups = ReadInt();
				int numBoneReference = ReadInt();

				geoset.Groups.resize( numGroups );

				ReadChar();  // {
				for ( int i = 0; i < numGroups; ++i )
				{
					auto& group = geoset.Groups[i];

					group.Indices[1] = -1;
					group.Indices[2] = -1;
					group.Indices[3] = -1;

					ReadString();  // Matrices
					ReadChar();  // {
					for ( int i = 0; i < 5; ++i )
					{
						auto token = ReadString();
						if ( token == "}" )
						{
							break;
						}
						else
						{
							stringstream ss( token );
							ss >> group.Indices[i];
						}
					}
				}
				ReadChar();  // }
			}
			else if ( context == "MinimumExtent" )
			{
				ReadVector3InBracket();
			}
			else if ( context == "MaximumExtent" )
			{
				ReadVector3InBracket();
			}
			else if ( context == "BoundsRadius" )
			{
				ReadDouble();
			}
			else if ( context == "Anim" )
			{
				ReadChar();
				DiscardBracketInChildrens();
			}
			else if ( context == "MaterialID" )
			{
				geoset.MaterialID = ReadInt();
			}
			else if ( context == "SelectionGroup" )
			{
				ReadInt();
			}
		}
	);

	return move( geoset );
}

vector<int> MDLParser::ReadTriangles( int numIndexes )
{
	vector<int> triangles( numIndexes );

	ReadChar();  // {
	for ( int i = 0; i < numIndexes; ++i )
	{
		triangles[i] = ReadInt();
	}
	ReadChar();  // }

	return move( triangles );
}

auto MDLParser::ReadTexture() -> tag_Texture
{
	tag_Texture txr;

	auto str = ReadString();
	if ( str == "Bitmap" )
	{
		ForEachContextsInBracket( [&]( string context )
			{
				if ( context == "Image" )
				{
					txr.Image = ReadString();
				}
				else if ( context == "ReplaceableId" )
				{
					txr.ReplaceableId = ReadInt();
				}
			}
		);
	}
	else
	{
		ForEachContextsInBracket();
	}
	
	return move( txr );
}

auto MDLParser::ReadMaterial() -> tag_Material
{
	tag_Material mtr;

	ForEachContextsInBracket( [&]( string name )
		{
			if ( name == "Layer" )
			{
				tag_Layer layer;

				ForEachContextsInBracket( [&]( string name )
					{
						if ( name == "static" )
						{
							ReadString();
							layer.TextureID = ReadInt();
						}
						else if ( name == "Alpha" )
						{
							ReadInt();
							ReadChar();
							DiscardBracketInChildrens();
						}
						else if ( name == "FilterMode" )
						{
							layer.FilterMode = ReadString();
						}
					}
				);

				mtr.Layer.push_back( move( layer ) );
			}
			else if ( name == "ConstantColor" )
			{
				mtr.ConstantColor = true;
			}
		}
	);

	return move( mtr );
}

auto MDLParser::ReadSequence() -> tag_Sequence
{
	tag_Sequence seq;

	ReadString();
	seq.Anim = ReadString();
	
	ForEachContextsInBracket( [&]( string context )
		{
			if ( context == "Interval" )
			{
				ReadVector2AsIntInBracket( &seq.Interval.Begin, &seq.Interval.End );
			}
			else if ( context == "NonLooping" )
			{
				seq.NonLooping = true;
			}
		}
	);

	return move( seq );
}

void MDLParser::ReadBone()
{
	if ( bones.size() == 0 )
	{
		bones.resize( model.NumBones + model.NumHelpers + model.NumAttachments );
	}

	tag_Bone bone;

	int objectId = 0;

	bone.Name = ReadString();

	ForEachContextsInBracket( [&]( string token )
		{
			if ( token == "ObjectId" )
			{
				objectId = ReadInt();
			}
			else if ( token == "Parent" )
			{
				bone.Parent = ReadInt();
			}
			else if ( token == "Rotation" )
			{
				int numKeyframes = ReadInt();

				bone.Rotation.Keyframes.resize( numKeyframes );
				ReadChar();  // {

				bone.Rotation.Method = ReadString(); 

				for ( int i = 0; i < numKeyframes; ++i )
				{
					auto& rot = bone.Rotation.Keyframes[i];

					rot.IntervalPos = ReadInt();
					rot.Value = ReadKeyframe4( bone.Rotation.Method );
				}

				ReadChar();  // }
			}
			else if ( token == "Scaling" )
			{
				int numKeyframes = ReadInt();

				bone.Scale.Keyframes.resize( numKeyframes );
				ReadChar();  // {

				bone.Scale.Method = ReadString();

				for ( int i = 0; i < numKeyframes; ++i )
				{
					auto& rot = bone.Scale.Keyframes[i];

					rot.IntervalPos = ReadInt();
					rot.Value = ReadKeyframe3( bone.Scale.Method );
				}

				ReadChar();  // }
			}
			else if ( token == "Translation" )
			{
				int numKeyframes = ReadInt();

				bone.Translation.Keyframes.resize( numKeyframes );
				ReadChar();  // {

				bone.Translation.Method = ReadString();

				for ( int i = 0; i < numKeyframes; ++i )
				{
					auto& rot = bone.Translation.Keyframes[i];

					rot.IntervalPos = ReadInt();
					rot.Value = ReadKeyframe3( bone.Translation.Method );
				}

				ReadChar();  // }
			}
		}
	);

	if ( auto it = boneNameReplace.find( bone.Name ); it != boneNameReplace.end() )
	{
		stringstream ss;
		ss << bone.Name << '_' << objectId;
		bone.Name = ss.str();
		boneNameReplace.insert( bone.Name );
	}
	else
	{
		boneNameReplace.insert( bone.Name );
	}

	bones[objectId] = move( bone );
}

void MDLParser::ReadGeosetAnim()
{
	ForEachContextsInBracket();
}

Vector3 MDLParser::ReadVector3InBracket()
{
	ReadChar();  // {
	auto x = ReadDouble();
	auto y = ReadDouble();
	auto z = ReadDouble();
	ReadChar();  // }
	return Vector3( x, y, z );
}

Vector2 MDLParser::ReadVector2InBracket()
{
	ReadChar();  // {
	auto x = ReadDouble();
	auto y = ReadDouble();
	ReadChar();  // }
	return Vector2( x, y );
}

Quaternion MDLParser::ReadVector4InBracket()
{
	Quaternion q;

	ReadChar(); // {
	q.X = ReadDouble();
	q.Y = ReadDouble();
	q.Z = ReadDouble();
	q.W = ReadDouble();
	ReadChar();  // }

	return q;
}

void MDLParser::ReadVector2AsIntInBracket( int* out1, int* out2 )
{
	ReadChar();
	*out1 = ReadInt();
	*out2 = ReadInt();
	ReadChar();
}

Vector3 MDLParser::ReadKeyframe3( const string& method )
{
	auto ret = ReadVector3InBracket();

	if ( method == "Hermite" )
	{
		ReadString();  // InTan
		ReadVector3InBracket();
		ReadString();  // OutTan
		ReadVector3InBracket();
	}

	return ret;
}

Quaternion MDLParser::ReadKeyframe4( const string& method )
{
	auto ret = ReadVector4InBracket();

	if ( method == "Hermite" )
	{
		ReadString();  // InTan
		ReadVector4InBracket();
		ReadString();  // OutTan
		ReadVector4InBracket();
	}

	return ret;
}

void MDLParser::ForEachContextsInBracket( function<void( string name )> contextFunctor )
{
	ReadChar();  // {
	while ( true )
	{
		if ( auto ch = ReadChar(); ch == '}' )
		{
			break;
		}
		else if ( ch == '{' )
		{
			DiscardBracketInChildrens();
		}
		else
		{
			Seekpos -= 1;
			contextFunctor( ReadString() );
		}
	}
}

void MDLParser::ForEachContextsInBracket()
{
	ReadChar();  // {
	while ( true )
	{
		if ( auto ch = ReadChar(); ch == '}' )
		{
			break;
		}
		else
		{
			Seekpos -= 1;
			ReadString();
		}
	}
}

void MDLParser::DiscardBracketInChildrens()
{
	while ( true )
	{
		auto ch = ReadChar();

		if ( ch == '{' )
		{
			DiscardBracketInChildrens();
		}
		else if ( ch == '}' )
		{
			break;
		}
	}
}

vector<RefPtr<Texture2D>> MDLParser::LoadTextures( String name, const path& resourcePath )
{
	vector<RefPtr<Texture2D>> txr( textures.size() );

	for ( int i = 0; i < ( int )textures.size(); ++i )
	{
		try
		{
			txr[i] = new Texture2D( String::Format( "{0}:Texture{1}", name, i ), resourcePath / textures[i].Image );
			AssetBundle::AddItem( txr[i] );
		}
		catch ( Exception* e )
		{
			txr[i] = nullptr;
			delete e;
		}
	}

	return move( txr );
}

vector<RefPtr<Material>> MDLParser::LoadMaterials( String name, const vector<RefPtr<Texture2D>>& txr )
{
	vector<RefPtr<Material>> mtr( materials.size() );

	for ( int i = 0; i < ( int )materials.size(); ++i )
	{
		mtr[i] = new Material( String::Format( "{0}:Texture{1}", name, i ) );

		if ( materials[i].Layer[0].TextureID != -1 )
		{
			mtr[i]->DiffuseMap = txr[materials[i].Layer[0].TextureID];
		}

		if ( materials[i].Layer[0].FilterMode == "Transparent" )
		{
			mtr[i]->Layer = RenderQueueLayer::Transparent;
		}
		else if ( materials[i].Layer[0].FilterMode == "Additive" )
		{
			mtr[i]->Layer = RenderQueueLayer::Additive;
		}

		AssetBundle::AddItem( mtr[i] );
	}

	return move( mtr );
}

void MDLParser::LoadBones( String name, RefPtr<GameObject> head )
{
	vector<RefPtr<GameObject>> gameObjects( bones.size() );

	RefPtr root = new GameObject();
	auto bone = root->AddComponent<Bone>();

	bone->Name = "root";
	bone->Index = bones.size();

	for ( int i = 0, count = ( int )bones.size(); i < count; ++i )
	{
		RefPtr go = new GameObject( name );

		Vector3 pivot;
		pivot.X = -pivots[i].Y;
		pivot.Y = pivots[i].Z;
		pivot.Z = pivots[i].X;

		auto trp = go->Transform;
		trp->Position = -pivot;

		auto bone = go->AddComponent<Bone>();
		bone->Name = bones[i].Name;
		bone->Index = i;
		
		gameObjects[i] = go;
	}

	for ( int i = 0, count = ( int )bones.size(); i < count; ++i )
	{
		auto par = bones[i].Parent;

		if ( par == -1 )
		{
			gameObjects[i]->Parent = root;
		}
		else
		{
			gameObjects[i]->Parent = gameObjects[par];
		}
	}

	root->Parent = head;
}

void MDLParser::LoadMeshes( String name, RefPtr<GameObject> head, const vector<RefPtr<Material>>& mtr )
{
	for ( int i = 0, count = geosets.size(); i < count; ++i )
	{
		auto& geoset = geosets[i];

		RefPtr gameObject = new GameObject();

		if ( model.NumBones == 0 )
		{
			vector<Vertex> vertexBuffer( geoset.Vertices.size() );
			for ( int i = 0, count = ( int )geoset.Vertices.size(); i < count; ++i )
			{
				auto& vertex = geoset.Vertices[i];
				auto& normal = geoset.Normals[i];
				auto& uv = geoset.TVertices[i];

				Vertex v;
				v.Pos = { -(float )vertex.Y, ( float )vertex.Z, ( float )vertex.X };
				v.Normal = { -( float )normal.Y, ( float )normal.Z, ( float )normal.X };
				v.Tex = { ( float )uv.X, ( float )uv.Y };
				v.Color = { 1.0f, 1.0f, 1.0f, 1.0f };

				vertexBuffer[i] = v;
			}

			vector<uint32> indexBuffer( geoset.Faces[0].size() );
			for ( int i = 0, count = ( int )geoset.Faces[0].size(); i < count; i += 1 )
			{
				auto& face = ( int( & )[3] )geoset.Faces[0][i];
				indexBuffer[i + 0] = face[0];
				indexBuffer[i + 1] = face[2];
				indexBuffer[i + 2] = face[1];
			}

			RefPtr mesh = new Mesh( String::Format( "{0}:Mesh{1}", name, i ), vertexBuffer, indexBuffer );
			AssetBundle::AddItem( mesh );

			auto meshFilter = gameObject->AddComponent<MeshFilter>();
			meshFilter->Mesh = mesh;
			auto meshRenderer = gameObject->AddComponent<MeshRenderer>();
			meshRenderer->Material = mtr[geoset.MaterialID];
		}
		else
		{
			vector<SkinnedVertex> vertexBuffer( geoset.Vertices.size() );
			for ( int i = 0, count = ( int )geoset.Vertices.size(); i < count; ++i )
			{
				auto& vertex = geoset.Vertices[i];
				auto& normal = geoset.Normals[i];
				auto& uv = geoset.TVertices[i];
				auto& group = geoset.Groups[geoset.VertexGroup[i]];

				SkinnedVertex v{ };
				v.Pos = { -( float )vertex.Y, ( float )vertex.Z, ( float )vertex.X };
				v.Normal = { -( float )normal.Y, ( float )normal.Z, ( float )normal.X };
				v.Tex = { ( float )uv.X, ( float )uv.Y };
				v.Color = { 1.0f, 1.0f, 1.0f, 1.0f };

				int numRef = 0;
				for ( int i = 0; i < 4; ++i )
				{
					if ( group.Indices[i] != -1 )
					{
						numRef += 1;
						v.Indices[i] = ( uint16 )group.Indices[i];
					}
				}

				float weight = 1.0f / numRef;
				for ( int i = 0; i < min( numRef, 3 ); ++i )
				{
					( ( float* )&v.Weights )[i] = weight;
				}

				vertexBuffer[i] = v;
			}

			vector<uint32> indexBuffer( geoset.Faces[0].size() );
			for ( int i = 0, count = ( int )geoset.Faces[0].size(); i < count; i += 3 )
			{
				auto& face = ( int( & )[3] )geoset.Faces[0][i];
				indexBuffer[i + 0] = face[0];
				indexBuffer[i + 1] = face[2];
				indexBuffer[i + 2] = face[1];
			}

			RefPtr mesh = new Mesh( String::Format( "{0}:Mesh{1}", name, i ), vertexBuffer, indexBuffer );
			AssetBundle::AddItem( mesh );

			auto meshRenderer = gameObject->AddComponent<SkinnedMeshRenderer>();
			meshRenderer->Material = mtr[geoset.MaterialID];
			meshRenderer->Mesh = mesh;
		}

		gameObject->Parent = head;
	}
}