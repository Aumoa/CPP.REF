#pragma once

namespace SC::Game::Details
{
	class M3DParser : public TextParser
	{
		struct tag_Header
		{
			int Materials;
			int Vertices;
			int Triangles;
			int Bones;
			int AnimationClips;
			bool IsSkinnedMesh;
		};

		struct tag_Material
		{
			float Ambient[3];
			float Diffuse[3];
			float Specular[3];
			float SpecPower;
			float Reflectivity[3];
			bool AlphaClip;
			std::string Effect;
			std::filesystem::path DiffuseMap;
			std::filesystem::path NormalMap;
		};

		struct tag_Subset
		{
			int SubsetID;
			int VertexStart;
			int VertexCount;
			int FaceStart;
			int FaceCount;
		};

		struct tag_Vertex
		{
			float Position[3];
			float Tangent[4];
			float Normal[3];
			float TexCoords[2];
			float BlendWeights[3];
			int BlendIndices[4];
		};

		struct tag_Triangle
		{
			int Face[3];
		};

		struct tag_BoneOffset
		{
			XMFLOAT4X4 BoneOffset;
		};

		struct tag_ParentIndex
		{
			int ParentIndexOfBone;
		};

		struct tag_Keyframe
		{
			float Time;
			float Pos[3];
			float Scale[3];
			float Quat[4];
		};

		struct tag_Keyframes
		{
			int NumKeyframes;
			std::vector<tag_Keyframe> Keyframes;
		};

		struct tag_AnimationClip
		{
			std::string Name;
			std::vector<tag_Keyframes> BoneKeyframes;
			double Duration;
		};

		const char* block = nullptr;
		int length = 0;
		RefPtr<IAsyncLoad> asyncLoad;

		double beginProg;
		double endProg;
		double prog;
		vs_property( double, Progress );
		void Progress_set( double value )
		{
			double len = endProg - beginProg;
			prog = beginProg + len * value;
			asyncLoad->Progress = prog;
		}
		double Progress_get()
		{
			return prog;
		}
		
	public:
		tag_Header Header;
		std::vector<tag_Material> Materials;
		std::vector<tag_Subset> Subsets;
		std::vector<tag_Vertex> Vertices;
		std::vector<tag_Triangle> Triangles;
		std::vector<tag_BoneOffset> BoneOffsets;
		std::vector<int> BoneHierarchy;
		std::vector<tag_AnimationClip> AnimationClips;

	public:
		M3DParser( const char* block, int length, RefPtr<IAsyncLoad> asyncLoad, double beginProg = 0, double endProg = 1 );
		M3DParser( std::filesystem::path filepath, RefPtr<IAsyncLoad> asyncLoad, double beginProg = 0, double endProg = 1 );

	private:
		void TryParse();

		void ReadHeader();
		void ReadMaterials();
		void ReadSubsets();
		void ReadVertices();
		void ReadTriangles();
		void ReadBones();
		void ReadHierarchy();
		void ReadAnimationClips();

		void ReadVector2( float out[2] );
		void ReadVector3( float out[3] );
		void ReadVector4( float out[4] );
		void ReadVector3From4( float out[3] );
		void ReadVector3AsInt( int out[3] );
		void ReadVector4AsInt( int out[4] );
	};
}