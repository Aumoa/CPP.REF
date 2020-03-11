using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

using namespace std;
using namespace std::filesystem;

M3DParser::M3DParser( const char* block, int length, RefPtr<IAsyncLoad> asyncLoad, double beginProg, double endProg ) : TextParser( " \n\t\r\n,{}" )
	, asyncLoad( asyncLoad )
	, beginProg( beginProg )
	, endProg( endProg )
{
	TryParse();
}

M3DParser::M3DParser( path filepath, RefPtr<IAsyncLoad> asyncLoad, double beginProg, double endProg ) : TextParser( " \n\t\r\n,{}" )
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

	double progressSteprate = 1024.0 / ( double )filesize * 0.1;
	bool cond = true;
	while ( cond )
	{
		int readlen = 1024;
		if ( filesize < 1024 )
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

void M3DParser::TryParse()
{
	ReadHeader();
	Progress = 0.11;
	ReadMaterials();
	Progress = 0.13;
	ReadSubsets();
	Progress = 0.15;
	ReadVertices();
	Progress = 0.65;
	ReadTriangles();
	Progress = 0.80;
	ReadBones();
	Progress = 0.84;
	ReadHierarchy();
	Progress = 0.86;
	ReadAnimationClips();
	Progress = 1;
}

void M3DParser::ReadHeader()
{
	ReadString();
	ReadString();
	Header.Materials = ReadInt();
	ReadString();
	Header.Vertices = ReadInt();
	ReadString();
	Header.Triangles = ReadInt();
	ReadString();
	Header.Bones = ReadInt();
	ReadString();
	Header.AnimationClips = ReadInt();

	if ( Header.Bones != 0 )
	{
		Header.IsSkinnedMesh = true;
	}
}

void M3DParser::ReadMaterials()
{
	ReadString();

	Materials.resize( Header.Materials );
	for ( int i = 0; i < Header.Materials; ++i )
	{
		tag_Material& material = Materials[i];

		ReadString();
		ReadVector3( material.Ambient );

		ReadString();
		ReadVector3( material.Diffuse );

		ReadString();
		ReadVector3( material.Specular );

		ReadString();
		material.SpecPower = ( float )ReadDouble();

		ReadString();
		ReadVector3( material.Reflectivity );

		ReadString();
		material.AlphaClip = ( bool )ReadInt();

		ReadString();
		material.Effect = ReadString();

		ReadString();
		material.DiffuseMap = ReadString();

		ReadString();
		material.NormalMap = ReadString();
	}
}

void M3DParser::ReadSubsets()
{
	ReadString();

	Subsets.resize( Header.Materials );
	for ( int i = 0; i < Header.Materials; ++i )
	{
		tag_Subset& subset = Subsets[i];

		ReadString();
		subset.SubsetID = ReadInt();

		ReadString();
		subset.VertexStart = ReadInt();

		ReadString();
		subset.VertexCount = ReadInt();

		ReadString();
		subset.FaceStart = ReadInt();

		ReadString();
		subset.FaceCount = ReadInt();
	}
}

void M3DParser::ReadVertices()
{
	ReadString();

	double step = 0.5 / Header.Vertices;

	Vertices.resize( Header.Vertices );
	for ( int i = 0; i < Header.Vertices; ++i )
	{
		tag_Vertex& vertex = Vertices[i];

		ReadString();
		ReadVector3( vertex.Position );
		
		ReadString();
		ReadVector3From4( vertex.Tangent );

		ReadString();
		ReadVector3( vertex.Normal );

		ReadString();
		ReadVector2( vertex.TexCoords );

		ReadString();
		ReadVector3From4( vertex.BlendWeights );

		ReadString();
		ReadVector4AsInt( vertex.BlendIndices );

		Progress = 0.15 + step * i;
	}
}

void M3DParser::ReadTriangles()
{
	ReadString();

	double step = 0.15 / Header.Triangles;

	Triangles.resize( Header.Triangles );
	for ( int i = 0; i < Header.Triangles; ++i )
	{
		tag_Triangle& triangle = Triangles[i];

		ReadVector3AsInt( triangle.Face );

		Progress = 0.65 + step * i;
	}
}

void M3DParser::ReadBones()
{
	ReadString();

	BoneOffsets.resize( Header.Bones );
	for ( int i = 0; i < Header.Bones; ++i )
	{
		tag_BoneOffset& bone = BoneOffsets[i];

		ReadString();
		ReadVector4( ( float* )bone.BoneOffset.m + 0 );
		ReadVector4( ( float* )bone.BoneOffset.m + 4 );
		ReadVector4( ( float* )bone.BoneOffset.m + 8 );
		ReadVector4( ( float* )bone.BoneOffset.m + 12 );
	}
}

void M3DParser::ReadHierarchy()
{
	ReadString();

	BoneHierarchy.resize( Header.Bones );
	for ( int i = 0; i < Header.Bones; ++i )
	{
		ReadString();
		BoneHierarchy[i] = ReadInt();
	}
}

void M3DParser::ReadAnimationClips()
{
	ReadString();

	double step0 = 0.14 / Header.AnimationClips;
	double step1 = step0 / Header.Bones;

	AnimationClips.resize( Header.AnimationClips );
	for ( int i = 0; i < Header.AnimationClips; ++i )
	{
		tag_AnimationClip& animationClip = AnimationClips[i];

		ReadString();
		animationClip.Name = ReadString();

		animationClip.BoneKeyframes.resize( Header.Bones );

		double rate = 0.86 + step0 * i;
		for ( int i = 0; i < Header.Bones; ++i )
		{
			tag_Keyframes& boneKeyframe = animationClip.BoneKeyframes[i];

			ReadString();
			ReadString();
			boneKeyframe.NumKeyframes = ReadInt();
			boneKeyframe.Keyframes.resize( boneKeyframe.NumKeyframes );
			for ( int i = 0; i < boneKeyframe.NumKeyframes; ++i )
			{
				auto& keyframe = boneKeyframe.Keyframes[i];

				ReadString();
				keyframe.Time = ( float )ReadDouble();
				ReadString();
				ReadVector3( keyframe.Pos );
				ReadString();
				ReadVector3( keyframe.Scale );
				ReadString();
				ReadVector4( keyframe.Quat );

				animationClip.Duration = max( animationClip.Duration, ( double )keyframe.Time );
			}

			Progress = rate + step1 * i;
		}
	}
}

void M3DParser::ReadVector2( float out[2] )
{
	out[0] = ( float )ReadDouble();
	out[1] = ( float )ReadDouble();
}

void M3DParser::ReadVector3( float out[3] )
{
	out[0] = ( float )ReadDouble();
	out[1] = ( float )ReadDouble();
	out[2] = ( float )ReadDouble();
}

void M3DParser::ReadVector4( float out[4] )
{
	out[0] = ( float )ReadDouble();
	out[1] = ( float )ReadDouble();
	out[2] = ( float )ReadDouble();
	out[3] = ( float )ReadDouble();
}

void M3DParser::ReadVector3From4( float out[3] )
{
	out[0] = ( float )ReadDouble();
	out[1] = ( float )ReadDouble();
	out[2] = ( float )ReadDouble();
	ReadDouble();
}

void M3DParser::ReadVector3AsInt( int out[3] )
{
	out[0] = ReadInt();
	out[1] = ReadInt();
	out[2] = ReadInt();
}

void M3DParser::ReadVector4AsInt( int out[4] )
{
	out[0] = ReadInt();
	out[1] = ReadInt();
	out[2] = ReadInt();
	out[3] = ReadInt();
}